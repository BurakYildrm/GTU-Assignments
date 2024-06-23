#include <lib/utility.h>
#include <multitasking.h>

using namespace myos;
using namespace myos::common;

Task::Task(GlobalDescriptorTable *gdt, void entrypoint()) {
    cpustate = (CPUState *) (stack + 4096 - sizeof(CPUState));

    cpustate->eax = 0;
    cpustate->ebx = 0;
    cpustate->ecx = 0;
    cpustate->edx = 0;

    cpustate->esi = 0;
    cpustate->edi = 0;
    cpustate->ebp = 0;

    cpustate->eip = (uint32_t) entrypoint;
    cpustate->cs = gdt->CodeSegmentSelector();
    cpustate->eflags = 0x202;

    pid = 0;
    ppid = 0;
    childMask = 0;
    state = READY;
    isParentWaiting = false;
}

Task::~Task() {}

TaskManager::TaskManager(GlobalDescriptorTable *gdt, MemoryManager *memoryManager) {
    numTasks = 0;
    currentTask = -1;
    nextPid = 0;
    this->gdt = gdt;

    for (int i = 0; i < MAX_TASKS; i++) {
        tasks[i] = (Task *) memoryManager->malloc(sizeof(Task));
        tasks[i]->state = FINISHED;
        tasks[i]->ppid = 0;
        tasks[i]->childMask = 0;
        tasks[i]->isParentWaiting = false;
    }
}

TaskManager::~TaskManager() {}

pid_t TaskManager::AddTask(Task *task) {
    if (numTasks >= MAX_TASKS)
        return false;

    task->pid = nextPid++;
    tasks[numTasks++] = task;
    return true;
}

int32_t TaskManager::FindTaskIndex(pid_t pid) {
    for (int32_t i = 0; i < numTasks; i++) {
        if (tasks[i]->pid == pid)
            return i;
    }

    return -1;
}

uint32_t TaskManager::GetCurrentPid() {
    return tasks[currentTask]->pid;
}

void TaskManager::TerminateTask() {
    Task *process = tasks[currentTask];

    process->state = FINISHED;

    if (process->isParentWaiting) {
        int32_t parentIndex = FindTaskIndex(process->ppid);

        if (parentIndex == -1) {
            return;
        }

        Task *parent = tasks[parentIndex];
        parent->childMask >>= 1;

        if (parent->childMask == 0) {
            parent->state = READY;
        }
    }
}

bool TaskManager::WaitTask(pid_t pid) {
    Task *current = tasks[currentTask];

    if (current->pid == pid || pid == 0) {
        return false;
    }

    if (pid == -1) {
        int32_t numChild = 0;

        for (int32_t i = 0; i < numTasks; i++) {
            if (tasks[i]->ppid == current->pid && tasks[i]->ppid != tasks[i]->pid && tasks[i]->state != FINISHED) {
                tasks[i]->isParentWaiting = true;
                numChild++;
            }
        }

        current->childMask = (1 << numChild) - 1;

        if (current->childMask == 0) {
            return false;
        }
    } else {
        int32_t index = FindTaskIndex(pid);

        if (index == -1) {
            return false;
        }

        if (numTasks <= index || tasks[index]->state == FINISHED) {
            return false;
        }

        current->childMask = 1;
        tasks[index]->isParentWaiting = true;
    }

    current->state = WAITING;
    return true;
}

uint32_t TaskManager::AlterTask(void entrypoint()) {
    tasks[currentTask]->state = READY;
    tasks[currentTask]->cpustate = (CPUState *) (tasks[currentTask]->stack + 4096 - sizeof(CPUState));

    tasks[currentTask]->cpustate->eax = 0;
    tasks[currentTask]->cpustate->ebx = 0;
    tasks[currentTask]->cpustate->ecx = 0;
    tasks[currentTask]->cpustate->edx = 0;

    tasks[currentTask]->cpustate->esi = 0;
    tasks[currentTask]->cpustate->edi = 0;
    tasks[currentTask]->cpustate->ebp = 0;

    tasks[currentTask]->cpustate->eip = (uint32_t) entrypoint;
    tasks[currentTask]->cpustate->cs = gdt->CodeSegmentSelector();
    tasks[currentTask]->cpustate->eflags = 0x202;

    return (uint32_t) tasks[currentTask]->cpustate;
}

uint32_t TaskManager::DuplicateTask(CPUState *cpu) {
    if (numTasks >= MAX_TASKS)
        return 0;

    tasks[numTasks]->state = READY;
    tasks[numTasks]->ppid = tasks[currentTask]->pid;
    tasks[numTasks]->pid = nextPid++;

    for (int i = 0; i < sizeof(tasks[currentTask]->stack); i++) {
        tasks[numTasks]->stack[i] = tasks[currentTask]->stack[i];
    }

    uint32_t currentTaskOffset = (((uint32_t) cpu - (uint32_t) tasks[currentTask]->stack));
    tasks[numTasks]->cpustate = (CPUState *) (((uint32_t) tasks[numTasks]->stack) + currentTaskOffset);

    tasks[numTasks]->cpustate->ecx = 0;
    numTasks++;
    return tasks[numTasks - 1]->pid;
}

void TaskManager::PrintProcessTable() {
    int8_t pidText[] = "PID";
    int8_t ppidText[] = "PPID";
    int8_t stateText[] = "STATE";
    int8_t lineSeparator = '-';

    int32_t pidLen = 5;
    int32_t ppidLen = 5;
    int32_t stateLen = 8;
    int32_t lineLen = pidLen + ppidLen + stateLen + 3;

    void *lineArgs[] = {&lineLen, &lineSeparator};
    printFormatted("\n%.*c\n", lineArgs);

    void *args[] = {&pidLen, pidText, &ppidLen, ppidText, &stateLen, stateText};
    printFormatted("%-*s %-*s %-*s\n", args);

    for (int i = 0; i < numTasks; i++) {
        pid_t pid = tasks[i]->pid;
        pid_t ppid = tasks[i]->ppid;
        TaskState state = tasks[i]->state;

        int8_t pidStr[MAX_INT_LEN + 1];
        int8_t ppidStr[MAX_INT_LEN + 1];

        itoa(pid, pidStr);
        itoa(ppid, ppidStr);

        void *entryArgs[] = {&pidLen, pidStr, &ppidLen, ppidStr, &stateLen, stateNames[state]};
        printFormatted("%-*s %-*s %-*s\n", entryArgs);
    }

    printFormatted("%.*c\n\n", lineArgs);
}

CPUState *TaskManager::Schedule(CPUState *cpustate) {
    if (numTasks <= 0)
        return cpustate;

    if (currentTask >= 0)
        tasks[currentTask]->cpustate = cpustate;

    int32_t nextTask = (currentTask + 1) % numTasks;

    while (tasks[nextTask]->state != READY && tasks[nextTask]->state != RUNNING) {
        nextTask = (nextTask + 1) % numTasks;
    }

    if (tasks[currentTask]->state == RUNNING) {
        tasks[currentTask]->state = READY;
    }

    currentTask = nextTask;
    tasks[currentTask]->state = RUNNING;
    // PrintProcessTable();
    return tasks[currentTask]->cpustate;
}

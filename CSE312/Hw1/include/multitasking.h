
#ifndef __MYOS__MULTITASKING_H
#define __MYOS__MULTITASKING_H

#include <common/types.h>
#include <gdt.h>
#include <memorymanagement.h>

#define MAX_TASKS 256
#define STACK_SIZE 4096
#define STATE_NUM 4

namespace myos {
    namespace hardwarecommunication {
        class InterruptHandler;
    }

    struct CPUState {
        common::uint32_t eax;
        common::uint32_t ebx;
        common::uint32_t ecx;
        common::uint32_t edx;

        common::uint32_t esi;
        common::uint32_t edi;
        common::uint32_t ebp;

        /*
        common::uint32_t gs;
        common::uint32_t fs;
        common::uint32_t es;
        common::uint32_t ds;
        */
        common::uint32_t error;

        common::uint32_t eip;
        common::uint32_t cs;
        common::uint32_t eflags;
        common::uint32_t esp;
        common::uint32_t ss;
    } __attribute__((packed));

    enum TaskState {
        READY,
        RUNNING,
        WAITING,
        FINISHED
    };

    class Task {
        friend class TaskManager;

      private:
        bool isParentWaiting;
        common::uint8_t stack[STACK_SIZE];
        common::uint32_t childMask;
        common::pid_t pid;
        common::pid_t ppid;
        TaskState state;
        CPUState *cpustate;

      public:
        Task(GlobalDescriptorTable *gdt, void entrypoint());
        ~Task();
    };

    class TaskManager {

      private:
        Task *tasks[MAX_TASKS];
        common::pid_t nextPid;
        common::int32_t numTasks;
        common::int32_t currentTask;
        GlobalDescriptorTable *gdt;
        char *stateNames[STATE_NUM] = {"READY", "RUNNING", "WAITING", "FINISHED"};

        common::int32_t FindTaskIndex(common::pid_t pid);
        void PrintProcessTable();

      public:
        TaskManager(GlobalDescriptorTable *gdt, MemoryManager *memoryManager);
        ~TaskManager();

        void TerminateTask();
        bool WaitTask(common::pid_t pid);
        common::pid_t AddTask(Task *task);
        common::pid_t DuplicateTask(CPUState *cpustate);
        common::pid_t GetCurrentPid();
        common::uint32_t AlterTask(void entrypoint());
        CPUState *Schedule(CPUState *cpustate);
    };

} // namespace myos

#endif

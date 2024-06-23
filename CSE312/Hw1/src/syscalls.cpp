#include <lib/utility.h>
#include <syscalls.h>

using namespace myos;
using namespace myos::common;
using namespace myos::hardwarecommunication;

pid_t myos::getpid() {
    int pid = -1;
    asm("int $0x80" : "=c"(pid) : "a"(Syscall::GETPID));
    return pid;
}

void myos::waitpid(pid_t pid) {
    asm("int $0x80" : : "a"(Syscall::WAITPID), "b"(pid));
}

void myos::exit() {
    asm("int $0x80" : : "a"(Syscall::EXIT));
}

void myos::fork(pid_t *pid) {
    asm("int $0x80" : "=c"(*pid) : "a"(Syscall::FORK));
}

void myos::execve(void entrypoint()) {
    asm("int $0x80" : : "a"(Syscall::EXECVE), "b"((uint32_t) entrypoint));
}

SyscallHandler::SyscallHandler(InterruptManager *interruptManager, uint8_t InterruptNumber)
    : InterruptHandler(interruptManager, InterruptNumber + interruptManager->HardwareInterruptOffset()) {}

SyscallHandler::~SyscallHandler() {}

common::uint32_t SyscallHandler::sys_getpid() {
    return interruptManager->taskManager->GetCurrentPid();
}

void SyscallHandler::sys_exit() {
    interruptManager->taskManager->TerminateTask();
}

bool SyscallHandler::sys_waitpid(uint32_t pid) {
    return interruptManager->taskManager->WaitTask((pid_t) pid);
}

myos::common::uint32_t SyscallHandler::sys_execve(common::uint32_t entrypoint) {
    return interruptManager->taskManager->AlterTask((void (*)()) entrypoint);
}

common::uint32_t SyscallHandler::sys_fork(CPUState *cpu) {
    return interruptManager->taskManager->DuplicateTask(cpu);
}

uint32_t SyscallHandler::HandleInterrupt(uint32_t esp) {
    CPUState *cpu = (CPUState *) esp;

    switch (cpu->eax) {
        case Syscall::EXECVE:
            esp = sys_execve(cpu->ebx);
            break;

        case Syscall::EXIT:
            sys_exit();
            return (uint32_t) interruptManager->taskManager->Schedule((CPUState *) esp);
            break;

        case Syscall::FORK:
            cpu->ecx = sys_fork(cpu);
            return (uint32_t) interruptManager->taskManager->Schedule((CPUState *) esp);
            break;

        case Syscall::GETPID:
            cpu->ecx = sys_getpid();
            break;

        case Syscall::WAITPID:
            if (sys_waitpid(cpu->ebx)) {
                return (uint32_t) interruptManager->taskManager->Schedule((CPUState *) esp);
            }
            break;

        default:
            break;
    }

    return esp;
}

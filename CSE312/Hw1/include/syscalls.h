#ifndef __MYOS__SYSCALLS_H
#define __MYOS__SYSCALLS_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <multitasking.h>

namespace myos {
    enum Syscall {
        EXECVE,
        EXIT,
        FORK,
        GETPID,
        WAITPID
    };

    common::pid_t getpid();
    void execve(void entrypoint());
    void exit();
    void fork(common::pid_t *pid);
    void waitpid(common::pid_t pid);

    class SyscallHandler : public hardwarecommunication::InterruptHandler {

      public:
        SyscallHandler(hardwarecommunication::InterruptManager *interruptManager, myos::common::uint8_t InterruptNumber);
        ~SyscallHandler();

        virtual myos::common::uint32_t HandleInterrupt(myos::common::uint32_t esp);

      private:
        void sys_exit();
        bool sys_waitpid(common::uint32_t pid);
        common::pid_t sys_getpid();
        common::uint32_t sys_execve(common::uint32_t entrypoint);
        common::uint32_t sys_fork(CPUState *cpu);
    };

} // namespace myos

#endif
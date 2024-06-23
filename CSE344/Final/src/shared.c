#include "shared.h"

FunctionStatus assign_signal_handler(int signo, void (*handler)(int)) {
    struct sigaction sa;
    sa.sa_handler = handler;
    sigfillset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(signo, &sa, NULL) == -1) {
        return FAILURE;
    }

    return SUCCESS;
}

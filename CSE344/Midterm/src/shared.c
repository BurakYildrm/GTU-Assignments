#include "shared.h"

const int command_args[COMMAND_NUM] = {
    [HELP] = 1,     [LIST] = 0,    [READ] = 2,        [WRITE] = 3, [UPLOAD] = 1,
    [DOWNLOAD] = 1, [ARCHIVE] = 1, [KILL_SERVER] = 0, [QUIT] = 0};

const char *command_names[COMMAND_NUM] = {[HELP] = "help",          [LIST] = "list",
                                          [READ] = "readF",         [WRITE] = "writeT",
                                          [UPLOAD] = "upload",      [DOWNLOAD] = "download",
                                          [ARCHIVE] = "archServer", [KILL_SERVER] = "killServer",
                                          [QUIT] = "quit"};

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
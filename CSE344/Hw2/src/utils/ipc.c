#include "utils/ipc.h"

void my_mkfifo(const char *path, mode_t mode) {
    if (mkfifo(path, mode) == -1) {
        perror("FIFO creation failed");
        exit(EXIT_FAILURE);
    }
}
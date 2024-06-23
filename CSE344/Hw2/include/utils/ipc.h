#ifndef IPC_H
#define IPC_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "utils/type.h"

#define FIFO_PERMS 0666

void my_mkfifo(const char *path, mode_t mode);

#endif

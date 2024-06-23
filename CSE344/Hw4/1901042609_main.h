#ifndef MAIN_H
#define MAIN_H

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "utils/io.h"
#include "utils/string.h"
#include "utils/type.h"

#define DIR_PERMS 0777
#define FILE_PERMS 0666
#define FIFO_PERMS 0666
#define BUFFER_SIZE 4096

typedef struct {
    int src_fd;
    int dest_fd;
    char src[PATH_MAX];
    char dest[PATH_MAX];
} CopyInfo;

Bool validate_args(int argc, char const *argv[]);
void cleanup();
void handle_directory(char *src, char *dest);
void handle_fifo(char *src, char *dest);
void handle_regular(char *src, char *dest);
void *manager(void *arg);
void *worker(void *arg);
void sigint_handler(int signo);

#endif

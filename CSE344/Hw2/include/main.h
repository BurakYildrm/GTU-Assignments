#ifndef MAIN_H
#define MAIN_H

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "utils/io.h"
#include "utils/ipc.h"
#include "utils/string.h"
#include "utils/type.h"

#define CMD_SIZE 9
#define MAX_INPUT_LEN 16
#define MAX_INPUT_VALUE 10
#define SLEEP_TIME 10
#define TERM_SIGS_NUM 6

typedef struct {
    int length;
    int value[MAX_INPUT_VALUE];
} FifoData;

void child_1();
void child_2();
void cleanup();
void sigchld_handler(int signo);
void sigusr1_handler(int signo);
void sigusr2_handler(int signo);
void term_sigs_handler(int signo);

#endif

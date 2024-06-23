#ifndef LOG_H
#define LOG_H

#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "utils/io.h"

#define LOG_FNAME_FORMAT "%Y-%m-%dT%H:%M:%S.log"
#define LOG_FNAME_LEN 24
#define MAX_LOG 256
#define TIMESTAMP_LEN 64

extern char logfile[LOG_FNAME_LEN];

char *get_timestamp(const char *format);
void init_log();
void log_message(const char *format, ...);

#endif

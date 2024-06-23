#ifndef LOG_H
#define LOG_H

#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "utils/io.h"
#include "utils/type.h"

#define LOG_FILENAME_FORMAT "%Y-%m-%dT%H:%M:%S.log"
#define LOG_FILENAME_LEN 64
#define LOG_LINE_TIMESTAMP_FORMAT "%Y-%m-%d %H:%M:%S"
#define LOG_LINE_TIMESTAMP_LEN 20
#define MAX_LOG_LINE 256
#define MAX_LOG_LEVEL_LEN 5

typedef enum {
    INFO = 0,
    ERROR = 1
} LogLevel;

extern char logfile[LOG_FILENAME_LEN];
extern const char *log_level_names[];

char *get_line_timestamp();
FunctionStatus init_log();
FunctionStatus log_message(LogLevel level, char *format, ...);

#endif
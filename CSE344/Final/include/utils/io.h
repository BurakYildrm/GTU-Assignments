#ifndef IO_H
#define IO_H

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "utils/array.h"
#include "utils/string.h"
#include "utils/type.h"

#define FILE_PERMS 0600
#define MAX_ERROR_LEN 256
#define MAX_INPUT_LEN (MAX_ARGS * (NAME_MAX + 1))
#define MAX_OUTPUT_LEN 1024
#define APPEND (O_WRONLY | O_CREAT | O_APPEND)
#define TRUNCATE (O_WRONLY | O_CREAT | O_TRUNC)
#define READ_ONLY (O_RDONLY | O_CREAT)
#define READ_WRITE (O_RDWR | O_CREAT | O_APPEND)
#define WRITE_ONLY (O_WRONLY | O_CREAT | O_APPEND)

extern pthread_mutex_t print_mutex;

FunctionStatus my_printf(const char *format, ...);
ssize_t my_vprintf(const char *format, va_list args);
void my_perror(const char *format, ...);
void my_vperror(const char *format, va_list args);

#endif

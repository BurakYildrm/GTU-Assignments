#ifndef IO_H
#define IO_H

#include <fcntl.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "utils/array.h"
#include "utils/string.h"
#include "utils/type.h"

#define MAX_ERROR_LEN 256
#define MAX_OUTPUT_LEN 1024
#define APPEND (O_WRONLY | O_CREAT | O_APPEND)
#define TRUNCATE (O_WRONLY | O_CREAT | O_TRUNC)

typedef enum {
    REGULAR,
    DIRECTORY,
    FIFO,
    UNKNOWN,
    INVALID
} FileType;

extern pthread_mutex_t print_mutex;

Bool file_exists(const char *filename);
FileType get_file_type(const char *filename);
FunctionStatus my_fgets(int fd, char *buffer, size_t size);
FunctionStatus my_printf(const char *format, ...);
ssize_t my_vprintf(const char *format, va_list args);
void display_error(const char *format, ...);
void vdisplay_error(const char *format, va_list args);

#endif

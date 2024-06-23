#ifndef IO_H
#define IO_H

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
#define OVERWRITE (O_WRONLY | O_CREAT | O_TRUNC)
#define READ_ONLY (O_RDONLY | O_CREAT)
#define READ_WRITE (O_RDWR | O_CREAT | O_APPEND)
#define WRITE_ONLY (O_WRONLY | O_CREAT | O_APPEND)

Bool file_exists(const char *filename);
FunctionStatus my_fgets(int fd, char *buffer, size_t size);
FunctionStatus my_printf(const char *format, ...);
ssize_t my_vprintf(const char *format, va_list args);
void display_error(const char *format, ...);
void vdisplay_error(const char *format, va_list args);

#endif

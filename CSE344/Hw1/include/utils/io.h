#ifndef IO_H
#define IO_H

#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "utils/array.h"
#include "utils/log.h"
#include "utils/string.h"

#define FILE_PERMS 0666
#define MAX_INPUT 128
#define MAX_OUTPUT 256
#define OVERWRITE (O_WRONLY | O_CREAT | O_TRUNC)
#define READ_ONLY (O_RDONLY | O_CREAT | O_APPEND)
#define READ_WRITE (O_RDWR | O_CREAT | O_APPEND)
#define WRITE_ONLY (O_WRONLY | O_CREAT | O_APPEND)

char *my_fgets(char *buffer, size_t size, int fd, char *filename);
int file_exists(const char *filename);
int my_open(const char *filename, int flags, mode_t mode);
ssize_t my_fprintf(int fd, const char *format, ...);
ssize_t my_printf(const char *format, ...);
ssize_t my_vprintf(const char *format, va_list args);
void my_close(int fd, const char *filename);

#endif

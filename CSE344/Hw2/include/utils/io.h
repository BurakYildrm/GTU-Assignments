#ifndef IO_H
#define IO_H

#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "utils/array.h"
#include "utils/string.h"

#define FILE_PERMS 0666
#define MAX_OUTPUT 256

char *my_fgets(char *buffer, size_t size, int fd);
int my_open(const char *filename, int flags, mode_t mode);
ssize_t my_printf(const char *format, ...);
ssize_t my_read(int fd, void *buffer, size_t size);
ssize_t my_vprintf(const char *format, va_list args);
ssize_t my_write(int fd, const void *buffer, size_t size);
void my_close(int fd);

#endif

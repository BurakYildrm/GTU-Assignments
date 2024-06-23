#ifndef IO_H
#define IO_H

#include <semaphore.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils/array.h"
#include "utils/string.h"

#define MAX_OUTPUT 256 // maximum output buffer size

extern sem_t print_mutex;

ssize_t my_printf(const char *format, ...);
ssize_t my_vprintf(const char *format, va_list args);

#endif

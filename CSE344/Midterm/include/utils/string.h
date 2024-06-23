#ifndef STRING_H
#define STRING_H

#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdlib.h>

#include "utils/type.h"

Bool is_integer(const char *str);
char *my_strtok(char *str, const char delim);
char *trim(char *str);
const char *my_strsignal(int signo);
int my_strcmp(const char *str1, const char *str2);
int parse_int(const char *str);
size_t my_snprintf(char *buffer, size_t size, const char *format, ...);
size_t my_strlen_utf8(const char *str);
size_t my_strlen(const char *str);
size_t my_vsnprintf(char *buffer, size_t size, const char *format, va_list args);

#endif

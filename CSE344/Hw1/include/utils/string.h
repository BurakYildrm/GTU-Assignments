#ifndef STRING_H
#define STRING_H

#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdlib.h>

#include "utils/log.h"

const char *my_strsignal(int signo);
char *my_strdup(const char *str);
char *my_strtok(char *str, const char delim);
char *ordinal_suffix(int num);
char *trim(char *str);
int is_integer(const char *str);
int my_strcmp(const char *str1, const char *str2);
int parse_int(const char *str);
size_t my_sprintf(char *buffer, size_t size, char *format, ...);
size_t my_strlen(const char *str);
size_t my_strlen_utf8(const char *str);
size_t my_vsprintf(char *buffer, size_t size, char *format, va_list args);

#endif

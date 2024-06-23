#ifndef STRING_H
#define STRING_H

#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int is_integer(const char *str);
int my_strcmp(const char *str1, const char *str2);
int parse_int(const char *str);
size_t my_strlen_utf8(const char *str);
size_t my_vsprintf(char *buffer, size_t size, char *format, va_list args);

#endif

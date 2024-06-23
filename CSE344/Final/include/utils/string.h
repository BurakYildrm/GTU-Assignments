#ifndef STRING_H
#define STRING_H

#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>

#include "utils/type.h"

#define MAX_IP_LENGTH 15
#define MAX_TIME_OPTION_LENGTH 10

Bool is_digit(const char c);
Bool is_integer(const char *str);
Bool is_valid_ip(const char *ip);
char *my_strtok(char *str, const char delim);
char *trim(char *str);
int my_strcmp(const char *str1, const char *str2);
int parse_int(const char *str);
size_t my_snprintf(char *buffer, size_t size, const char *format, ...);
size_t my_strcpy(char *dest, const char *src);
size_t my_strftime(char *buffer, size_t size, const char *format, const struct tm *tm_info);
size_t my_strlen_utf8(const char *str);
size_t my_strlen(const char *str);
size_t my_vsnprintf(char *buffer, size_t size, const char *format, va_list args);

#endif

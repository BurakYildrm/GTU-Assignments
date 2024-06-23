#ifndef STRING_H
#define STRING_H

#include <stdarg.h>
#include <stdio.h>

size_t my_strlen_utf8(const char *str);
size_t my_vsprintf(char *buffer, size_t size, char *format, va_list args);

#endif

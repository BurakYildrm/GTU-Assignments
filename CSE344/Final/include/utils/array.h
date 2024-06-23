#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>

#include "utils/string.h"

int indexof_char(char *arr, size_t size, char value);
int indexof_struct(const void *arr, size_t size, size_t elem_size, const void *value,
                   int (*cmp)(const void *, const void *));

#endif

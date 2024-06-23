#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>

#include "utils/string.h"

int indexof_int(int *arr, size_t size, int value);
int indexof_char(char *arr, size_t size, char value);
int indexof_str(char **arr, size_t size, const char *value);
int indexof_struct(const void *arr, size_t size, size_t elem_size, const void *value,
                   int (*cmp)(const void *, const void *));
int rm_dups_int(int *arr, size_t size);
int rm_dups_char(char *arr, size_t size);
int rm_dups_str(char **arr, size_t size);

#endif

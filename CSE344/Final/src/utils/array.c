#include "utils/array.h"

int indexof_char(char *arr, size_t size, char value) {
    for (size_t i = 0; i < size; i++) {
        if (arr[i] == value) {
            return i;
        }
    }

    return -1;
}

int indexof_struct(const void *arr, size_t size, size_t elem_size, const void *value, int (*cmp)(const void *, const void *)) {
    for (size_t i = 0; i < size; i++) {
        if (cmp((const void *) ((const char *) arr + i * elem_size), value) == 0) {
            return i;
        }
    }

    return -1;
}

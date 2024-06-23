#include "utils/array.h"

int indexof_int(int *arr, size_t size, int value) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) {
            return i;
        }
    }

    return -1;
}

int indexof_char(char *arr, size_t size, char value) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) {
            return i;
        }
    }

    return -1;
}

int indexof_str(char **arr, size_t size, const char *value) {
    for (int i = 0; i < size; i++) {
        if (my_strcmp(arr[i], value) == 0) {
            return i;
        }
    }

    return -1;
}

int indexof_struct(const void *arr, size_t size, size_t elem_size, const void *value,
                   int (*cmp)(const void *, const void *)) {
    for (int i = 0; i < size; i++) {
        if (cmp((const void *) ((const char *) arr + i * elem_size), value) == 0) {
            return i;
        }
    }

    return -1;
}

int rm_dups_int(int *arr, size_t size) {
    int write_index = 0;

    for (int i = 0; i < size; i++) {
        int is_duplicate = 0;
        for (int j = 0; j < write_index; j++) {
            if (arr[i] == arr[j]) {
                is_duplicate = 1;
                break;
            }
        }
        if (!is_duplicate) {
            arr[write_index++] = arr[i];
        }
    }

    return write_index;
}

int rm_dups_char(char *arr, size_t size) {
    int write_index = 0;

    for (int i = 0; i < size; i++) {
        int is_duplicate = 0;
        for (int j = 0; j < write_index; j++) {
            if (arr[i] == arr[j]) {
                is_duplicate = 1;
                break;
            }
        }
        if (!is_duplicate) {
            arr[write_index++] = arr[i];
        }
    }

    return write_index;
}

int rm_dups_str(char **arr, size_t size) {
    int write_index = 0;

    for (int i = 0; i < size; i++) {
        int is_duplicate = 0;
        for (int j = 0; j < write_index; j++) {
            if (my_strcmp(arr[i], arr[j]) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        if (!is_duplicate) {
            arr[write_index++] = arr[i];
        }
    }

    return write_index;
}
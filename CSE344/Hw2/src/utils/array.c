#include "utils/array.h"

int indexof_char(char *arr, size_t size, char value) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) {
            return i;
        }
    }

    return -1;
}
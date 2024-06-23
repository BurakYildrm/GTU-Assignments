#include "utils/memory.h"

void *my_malloc(size_t size) {
    void *ptr = malloc(size);

    if (ptr == NULL) {
        log_message("[%s] Failed to allocate memory in the child process with PID %ld.\n",
                    get_timestamp("%T"), (long) getpid());
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

void *my_realloc(void *ptr, size_t size) {
    void *new_ptr = realloc(ptr, size);

    if (new_ptr == NULL) {
        log_message("[%s] Failed to reallocate memory in the child process with PID %ld.\n",
                    get_timestamp("%T"), (long) getpid());
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    return new_ptr;
}
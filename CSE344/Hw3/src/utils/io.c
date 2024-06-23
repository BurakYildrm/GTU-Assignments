#include "utils/io.h"

sem_t print_mutex;

ssize_t my_printf(const char *format, ...) {
    sem_wait(&print_mutex);
    va_list args;
    va_start(args, format);

    ssize_t bytes_written = my_vprintf(format, args);

    va_end(args);
    sem_post(&print_mutex);
    return bytes_written;
}

ssize_t my_vprintf(const char *format, va_list args) {
    char buffer[MAX_OUTPUT];
    int bytes_formatted = my_vsprintf(buffer, sizeof(buffer), format, args);
    ssize_t bytes_written = write(STDOUT_FILENO, buffer, bytes_formatted);

    if (bytes_written == -1) {
        perror("Error writing to output");
        exit(EXIT_FAILURE);
    }

    return bytes_written;
}
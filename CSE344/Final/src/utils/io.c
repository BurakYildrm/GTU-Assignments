#include "utils/io.h"

pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

FunctionStatus my_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    ssize_t bytes_written = my_vprintf(format, args);

    va_end(args);
    return bytes_written == -1 ? FAILURE : SUCCESS;
}

ssize_t my_vprintf(const char *format, va_list args) {
    char buffer[MAX_OUTPUT_LEN + 1];
    int bytes_formatted = my_vsnprintf(buffer, sizeof(buffer), format, args);

    pthread_mutex_lock(&print_mutex);
    ssize_t bytes_written = write(STDOUT_FILENO, buffer, bytes_formatted);
    pthread_mutex_unlock(&print_mutex);

    return bytes_written;
}

void my_perror(const char *format, ...) {
    va_list args;
    va_start(args, format);

    my_vperror(format, args);

    va_end(args);
}

void my_vperror(const char *format, va_list args) {
    char buffer[MAX_ERROR_LEN];
    my_vsnprintf(buffer, sizeof(buffer), format, args);
    perror(buffer);
}

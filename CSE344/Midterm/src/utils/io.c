#include "utils/io.h"

Bool file_exists(const char *filename) {
    struct stat buffer;
    return stat(filename, &buffer) == 0 ? TRUE : FALSE;
}

FunctionStatus my_fgets(int fd, char *buffer, size_t size) {
    ssize_t bytes = read(fd, buffer, size - 1);

    if (bytes == 0) {
        return END_OF_FILE;
    }

    if (bytes == -1) {
        return FAILURE;
    }

    buffer[fd == STDIN_FILENO ? bytes - 1 : bytes] = '\0';

    if (fd != STDIN_FILENO) {
        int newline = indexof_char(buffer, size, '\n');

        if (newline != -1) {
            buffer[newline] = '\0';
            lseek(fd, -(bytes - newline - 1), SEEK_CUR);
        }
    }

    return SUCCESS;
}

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
    ssize_t bytes_written = write(STDOUT_FILENO, buffer, bytes_formatted);

    return bytes_written;
}

void display_error(const char *format, ...) {
    va_list args;
    va_start(args, format);

    vdisplay_error(format, args);

    va_end(args);
}

void vdisplay_error(const char *format, va_list args) {
    char buffer[MAX_ERROR_LEN];
    my_vsnprintf(buffer, sizeof(buffer), format, args);
    perror(buffer);
}
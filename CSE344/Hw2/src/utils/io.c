#include "utils/io.h"

pid_t parent_pid;

char *my_fgets(char *buffer, size_t size, int fd) {
    ssize_t bytes = read(fd, buffer, size - 1);

    if (bytes == 0) {
        return NULL;
    }

    if (bytes == -1) {
        perror("Error reading input");
        kill(parent_pid, SIGUSR2);
    }

    buffer[fd == STDIN_FILENO ? bytes - 1 : bytes] = '\0';

    if (fd != STDIN_FILENO) {
        int newline = indexof_char(buffer, size, '\n');

        if (newline != -1) {
            buffer[newline] = '\0';
            lseek(fd, -(bytes - newline - 1), SEEK_CUR);
        }
    }

    return buffer;
}

int my_open(const char *filename, int flags, mode_t mode) {
    int fd = open(filename, flags, mode);

    if (fd == -1) {
        perror("Error opening file");
        kill(parent_pid, SIGUSR2);
    }

    return fd;
}

ssize_t my_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    ssize_t bytes_written = my_vprintf(format, args);

    va_end(args);
    return bytes_written;
}

ssize_t my_read(int fd, void *buffer, size_t size) {
    ssize_t bytes_read = read(fd, buffer, size);

    if (bytes_read == -1) {
        perror("Error reading file");
        kill(parent_pid, SIGUSR2);
    }

    return bytes_read;
}

ssize_t my_vprintf(const char *format, va_list args) {
    char buffer[MAX_OUTPUT];
    int bytes_formatted = my_vsprintf(buffer, sizeof(buffer), format, args);
    ssize_t bytes_written = write(STDOUT_FILENO, buffer, bytes_formatted);

    if (bytes_written == -1) {
        perror("Error writing to output");
        kill(parent_pid, SIGUSR2);
    }

    return bytes_written;
}

ssize_t my_write(int fd, const void *buffer, size_t size) {
    ssize_t bytes_written = write(fd, buffer, size);

    if (bytes_written == -1) {
        perror("Error writing to file");
        kill(parent_pid, SIGUSR2);
    }

    return bytes_written;
}

void my_close(int fd) {
    int status = close(fd);

    if (status == -1) {
        perror("Error closing file");
        kill(parent_pid, SIGUSR2);
    }
}
#include "utils/io.h"

char *my_fgets(char *buffer, size_t size, int fd, char *filename) {
    ssize_t bytes = read(fd, buffer, size - 1);

    if (bytes == 0) {
        return NULL;
    }

    if (bytes == -1) {
        filename == NULL
            ? log_message("[%s] Failed to read from the file descriptor %d in the child "
                          "process with PID %ld.\n",
                          get_timestamp("%T"), fd, (long) getpid())
            : log_message("[%s] Failed to read from the file \"%s\" in the child "
                          "process with PID %ld.\n",
                          get_timestamp("%T"), filename, (long) getpid());
        perror("Error reading input");
        exit(EXIT_FAILURE);
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

int file_exists(const char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

int my_open(const char *filename, int flags, mode_t mode) {
    int fd = open(filename, flags, mode);

    if (fd == -1) {
        log_message("[%s] Failed to open the file \"%s\" in the child "
                    "process with PID %ld.\n",
                    get_timestamp("%T"), filename, (long) getpid());
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    return fd;
}

ssize_t my_fprintf(int fd, const char *format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[MAX_OUTPUT];
    int bytes_formatted = my_vsprintf(buffer, sizeof(buffer), format, args);
    ssize_t bytes_written = write(fd, buffer, bytes_formatted);

    va_end(args);
    return bytes_written;
}

ssize_t my_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    ssize_t bytes_written = my_vprintf(format, args);

    va_end(args);
    return bytes_written;
}

ssize_t my_vprintf(const char *format, va_list args) {
    char buffer[MAX_OUTPUT];
    int bytes_formatted = my_vsprintf(buffer, sizeof(buffer), format, args);
    ssize_t bytes_written = write(STDOUT_FILENO, buffer, bytes_formatted);

    if (bytes_written == -1) {
        log_message("[%s] Failed to write to the standard output in the child "
                    "process with PID %ld.\n",
                    get_timestamp("%T"), (long) getpid());
        perror("Error writing to output");
        exit(EXIT_FAILURE);
    }

    return bytes_written;
}

void my_close(int fd, const char *filename) {
    int status = close(fd);

    if (status == -1) {
        filename == NULL ? log_message("[%s] Failed to close the file descriptor %d in the child "
                                       "process with PID %ld.\n",
                                       get_timestamp("%T"), fd, (long) getpid())
                         : log_message("[%s] Failed to close the file \"%s\" in the child "
                                       "process with PID %ld.\n",
                                       get_timestamp("%T"), filename, (long) getpid());
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }
}
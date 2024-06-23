#include "utils/log.h"

char logfile[LOG_FNAME_LEN];

char *get_timestamp(const char *format) {
    time_t now = time(NULL);
    static char timestamp[TIMESTAMP_LEN];

    strftime(timestamp, TIMESTAMP_LEN, format, localtime(&now));
    return timestamp;
}

void init_log() {
    time_t now = time(NULL);
    strftime(logfile, LOG_FNAME_LEN, LOG_FNAME_FORMAT, localtime(&now));

    int fd = open(logfile, WRITE_ONLY, FILE_PERMS);

    if (fd == -1) {
        perror("Error opening log file");
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        perror("Error closing log file");
        exit(EXIT_FAILURE);
    }
}

void log_message(const char *format, ...) {
    char buffer[MAX_LOG];
    int fd;
    ssize_t bytes_written;
    va_list args;
    va_start(args, format);

    fd = open(logfile, WRITE_ONLY, FILE_PERMS);
    if (fd == -1) {
        perror("Error opening log file");
        exit(EXIT_FAILURE);
    }

    size_t bytes_formatted = my_vsprintf(buffer, MAX_LOG, format, args);
    bytes_written = write(fd, buffer, bytes_formatted);

    if (bytes_written == -1) {
        perror("Error writing to log file");
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        perror("Error closing log file");
        exit(EXIT_FAILURE);
    }

    va_end(args);
}
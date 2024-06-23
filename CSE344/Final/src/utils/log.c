#include "utils/log.h"

const char *log_level_names[] = {[INFO] = "INFO", [ERROR] = "ERROR"};
char logfile[LOG_FILENAME_LEN] = LOG_FILENAME_FORMAT;

char *get_line_timestamp(void) {
    time_t now = time(NULL);
    static char timestamp[LOG_LINE_TIMESTAMP_LEN];

    my_strftime(timestamp, LOG_LINE_TIMESTAMP_LEN, LOG_LINE_TIMESTAMP_FORMAT, localtime(&now));
    return timestamp;
}

FunctionStatus init_log(void) {
    // my_snprintf(logfile, LOG_FILENAME_LEN, LOG_FILENAME_FORMAT);
    int fd = open(logfile, TRUNCATE, FILE_PERMS);

    if (fd == -1) {
        my_perror("Error opening the log file %s", logfile);
        return FAILURE;
    }

    if (close(fd) == -1) {
        my_perror("Error closing the log file %s", logfile);
        return FAILURE;
    }

    return SUCCESS;
}

FunctionStatus log_message(LogLevel level, char *format, ...) {
    char buffer[MAX_LOG_LINE];
    char combined_format[MAX_LOG_LINE];
    int fd;
    ssize_t bytes_written;

    va_list args;
    va_start(args, format);

    fd = open(logfile, WRITE_ONLY, FILE_PERMS);

    if (fd == -1) {
        my_perror("Error opening the log file %s", logfile);
        return FAILURE;
    }

    my_snprintf(combined_format, MAX_LOG_LINE, "[%s] [%s] %s\n", get_line_timestamp(), log_level_names[level], format);
    size_t bytes_formatted = my_vsnprintf(buffer, MAX_LOG_LINE, combined_format, args);

    flock(fd, LOCK_EX);
    bytes_written = write(fd, buffer, bytes_formatted);
    flock(fd, LOCK_UN);

    if (bytes_written == -1) {
        my_perror("Error writing to the log file %s", logfile);
        return FAILURE;
    }

    if (close(fd) == -1) {
        my_perror("Error closing the log file %s", logfile);
        return FAILURE;
    }

    va_end(args);
    return SUCCESS;
}

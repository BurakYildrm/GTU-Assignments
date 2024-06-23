#include "utils/string.h"

const char *my_strsignal(int signo) {
    switch (signo) {
    case SIGBUS:
        return "SIGBUS";
    case SIGCHLD:
        return "SIGCHLD";
    case SIGHUP:
        return "SIGHUP";
    case SIGINT:
        return "SIGINT";
    case SIGPIPE:
        return "SIGPIPE";
    case SIGQUIT:
        return "SIGQUIT";
    case SIGSEGV:
        return "SIGSEGV";
    case SIGTERM:
        return "SIGTERM";
    case SIGUSR1:
        return "SIGUSR1";
    case SIGUSR2:
        return "SIGUSR2";
    default:
        return "Unknown signal";
    }
}

char *my_strdup(const char *str) {
    size_t length = my_strlen(str);
    char *new_str;

    new_str = (char *) malloc(length + 1);

    if (new_str == NULL) {
        log_message(
            "[%s] Failed to allocate memory for string duplication in the child process with "
            "PID %ld.\n",
            get_timestamp("%T"), (long) getpid());
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < length; i++) {
        new_str[i] = str[i];
    }

    new_str[length] = '\0';
    return new_str;
}

char *my_strtok(char *str, const char delim) {
    static char *p;
    char *start;

    if (str == NULL && *p == '\0') {
        return NULL;
    }

    if (str != NULL) {
        p = str;
    }

    while (*p != '\0' && (*p == delim || *p == '\n' || *p == '\r' || *p == '\t' || *p == ' ')) {
        p++;
    }

    if (*p == '\0') {
        return NULL;
    }

    start = p;
    while (*p != '\0') {
        if (*p == delim) {
            *p = '\0';
            p++;
            return start;
        }
        p++;
    }

    return start;
}

char *ordinal_suffix(int num) {
    int last_two_digits = num % 100;
    if (last_two_digits == 11 || last_two_digits == 12 || last_two_digits == 13) {
        return "th";
    }

    switch (num % 10) {
    case 1:
        return "st";
    case 2:
        return "nd";
    case 3:
        return "rd";
    default:
        return "th";
    }
}

char *trim(char *str) {
    char *end;

    if (str == NULL) {
        return NULL;
    }

    while ((unsigned char) *str == ' ') {
        str++;
    }

    if (*str == 0) {
        return str;
    }

    end = str + my_strlen(str) - 1;
    while (end > str && ((unsigned char) *end == ' ')) {
        end--;
    }

    end[1] = '\0';
    return str;
}

int is_integer(const char *str) {
    if (*str == '\0') {
        return 0;
    }

    while (*str) {
        if (*str < '0' || *str > '9') {
            return 0;
        }
        str++;
    }

    return 1;
}

int my_strcmp(const char *str1, const char *str2) {
    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 != *str2) {
            return *str1 - *str2;
        }
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

int parse_int(const char *str) {
    int is_negative = 0;

    if (*str == '-') {
        str++;
        is_negative = 1;
    }

    return is_integer(str) ? atoi(is_negative ? --str : str) : INT_MIN;
}

size_t my_sprintf(char *buffer, size_t size, char *format, ...) {
    va_list args;
    va_start(args, format);

    size_t bytes = my_vsprintf(buffer, size, format, args);

    va_end(args);
    return bytes;
}

size_t my_strlen(const char *str) {
    size_t length = 0;
    while (*str != '\0') {
        length++;
        str++;
    }
    return length;
}

size_t my_strlen_utf8(const char *str) {
    size_t length = 0;
    while (*str != '\0') {
        if ((*str & 0xC0) != 0x80) { // UTF-8 character
            length++;
        }
        str++;
    }
    return length;
}

size_t my_vsprintf(char *buffer, size_t size, char *format, va_list args) {
    for (int i = 0; i < size; i++) {
        buffer[i] = '\0';
    }

    const char *p = format;
    size_t bytes = 0;

    while (*p && bytes < size - 2) {
        if (*p == '%') {
            if (*(p + 1) == 's') {
                p++;
                char *str = va_arg(args, char *);

                if (str == NULL) {
                    continue;
                }

                while (*str && bytes < size - 1) {
                    buffer[bytes++] = *str++;
                }
            } else if (*(p + 1) == 'd') {
                p++;
                int num = va_arg(args, int);
                int digits = 0;
                int temp = num;

                if (num < 0) {
                    buffer[bytes++] = '-';
                    num = -num;
                    temp = num;
                }

                while (temp) {
                    temp /= 10;
                    digits++;
                }

                if (num == 0) {
                    buffer[bytes++] = '0';
                } else {
                    temp = num;
                    int tempDigits = digits;
                    while (temp) {
                        buffer[bytes + tempDigits - 1] = '0' + temp % 10;
                        temp /= 10;
                        tempDigits--;
                    }
                    bytes += digits;
                }
            } else if (*(p + 1) == 'c') {
                p++;
                char c = va_arg(args, char);
                buffer[bytes++] = c;
            } else if (*(p + 1) == 'l' && *(p + 2) == 'd') {
                p += 2;
                long num = va_arg(args, long);
                int digits = 0;
                long temp = num;

                if (num < 0) {
                    buffer[bytes++] = '-';
                    num = -num;
                    temp = num;
                }

                while (temp) {
                    temp /= 10;
                    digits++;
                }

                if (num == 0) {
                    buffer[bytes++] = '0';
                } else {
                    temp = num;
                    int tempDigits = digits;
                    while (temp) {
                        buffer[bytes + tempDigits - 1] = '0' + temp % 10;
                        temp /= 10;
                        tempDigits--;
                    }
                    bytes += digits;
                }
            } else if (*(p + 1) == '-' && *(p + 2) == '*' && *(p + 3) == 's') {
                p += 3;
                int width = va_arg(args, int);
                char *str = va_arg(args, char *);

                if (str == NULL) {
                    continue;
                }

                int str_length = my_strlen_utf8(str);

                while (*str && bytes < size - 1) {
                    buffer[bytes++] = *str++;
                }

                if (str_length < width) {
                    for (int i = 0; i < width - str_length; i++) {
                        buffer[bytes++] = ' ';
                    }
                }
            } else if (*(p + 1) == '.' && *(p + 2) == '*' && *(p + 3) == 'c') {
                p += 3;
                int width = va_arg(args, int);
                char c = va_arg(args, int);

                for (int i = 0; i < width; i++) {
                    buffer[bytes++] = c;
                }
            } else if (*(p + 1) == '.' && *(p + 2) == '*' && *(p + 3) == 's') {
                p += 3;
                int width = va_arg(args, int);
                char *str = va_arg(args, char *);
                char *tmp = str;

                if (str == NULL) {
                    continue;
                }

                for (int i = 0; i < width; i++) {
                    tmp = str;
                    while (*tmp && bytes < size - 1) {
                        buffer[bytes++] = *tmp++;
                    }
                }
            }
        } else {
            buffer[bytes++] = *p;
        }
        p++;
    }

    buffer[bytes] = '\0';
    return bytes;
}
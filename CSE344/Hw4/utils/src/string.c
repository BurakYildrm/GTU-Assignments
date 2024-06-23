#include "utils/string.h"

Bool is_integer(const char *str) {
    if (*str == '\0') {
        return FALSE;
    }

    while (*str) {
        if (*str < '0' || *str > '9') {
            return 0;
        }
        str++;
    }

    return TRUE;
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

    return is_integer(str) == TRUE ? atoi(is_negative ? --str : str) : INT_MIN;
}

size_t my_snprintf(char *buffer, size_t size, const char *format, ...) {
    va_list args;
    va_start(args, format);

    size_t bytes = my_vsnprintf(buffer, size, format, args);

    va_end(args);
    return bytes;
}

size_t my_strcpy(char *dest, const char *src) {
    size_t length = 0;

    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
        length++;
    }

    *dest = '\0';
    return length;
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

size_t my_vsnprintf(char *buffer, size_t size, const char *format, va_list args) {
    for (int i = 0; i < size; i++) {
        buffer[i] = '\0';
    }

    const char *p = format;
    size_t bytes = 0;
    size_t format_size = my_strlen(format);

    for (int i = 0; i < format_size && bytes < size - 1; i++) {
        if (p[i] == '%') {
            if (i < format_size - 1 && p[i + 1] == 's') {
                i++;
                char *str = va_arg(args, char *);

                if (str == NULL) {
                    i++;
                    continue;
                }

                while (*str && bytes < size - 1) {
                    buffer[bytes++] = *str++;
                }
            } else if (i < format_size - 1 && p[i + 1] == 'd') {
                i++;
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
            } else if (i < format_size - 1 && p[i + 1] == 'c') {
                i++;
                char c = va_arg(args, int);
                buffer[bytes++] = c;
            } else if (i < format_size - 2 && p[i + 1] == 'l' && p[i + 2] == 'd') {
                i += 2;
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
            } else if (i < format_size - 3 && p[i + 1] == '-' && p[i + 2] == '*' && p[i + 3] == 's') {
                i += 3;
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
            } else if (i < format_size - 3 && p[i + 1] == '.' && p[i + 2] == '*' && p[i + 3] == 'c') {
                i += 3;
                int width = va_arg(args, int);
                char c = va_arg(args, int);

                for (int i = 0; i < width; i++) {
                    buffer[bytes++] = c;
                }
            } else if (i < format_size - 3 && p[i + 1] == '.' && p[i + 2] == '*' && p[i + 3] == 's') {
                i += 3;
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
            } else if (i < format_size - 3 && p[i + 1] >= '0' && p[i + 1] <= '9' && p[i + 2] >= '0' &&
                       p[i + 2] <= '9' && p[i + 3] == 'd') {
                char filler = p[i + 1];
                int width = p[i + 2] - '0';
                i += 3;
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
                    for (int i = 0; i < width - 1; i++) {
                        buffer[bytes++] = filler;
                    }
                    buffer[bytes++] = '0';
                } else {
                    temp = num;
                    int tempDigits = digits;

                    if (digits < width) {
                        for (int i = 0; i < width - digits; i++) {
                            buffer[bytes++] = filler;
                        }
                    }

                    while (temp) {
                        buffer[bytes + tempDigits - 1] = '0' + temp % 10;
                        temp /= 10;
                        tempDigits--;
                    }
                    bytes += digits;
                }
            } else if (i < format_size - 4 && p[i + 1] >= '0' && p[i + 1] <= '9' && p[i + 2] >= '0' &&
                       p[i + 2] <= '9' && p[i + 3] == 'l' && p[i + 4] == 'd') {
                char filler = p[i + 1];
                int width = p[i + 2] - '0';
                i += 4;
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
                    for (int i = 0; i < width - 1; i++) {
                        buffer[bytes++] = filler;
                    }
                    buffer[bytes++] = '0';
                } else {
                    temp = num;
                    int tempDigits = digits;

                    if (digits < width) {
                        for (int i = 0; i < width - digits; i++) {
                            buffer[bytes++] = filler;
                        }
                    }

                    while (temp) {
                        buffer[bytes + tempDigits - 1] = '0' + temp % 10;
                        temp /= 10;
                        tempDigits--;
                    }
                    bytes += digits;
                }
            } else {
                buffer[bytes++] = p[i];
            }
        } else {
            buffer[bytes++] = p[i];
        }
    }

    buffer[bytes] = '\0';
    return bytes;
}
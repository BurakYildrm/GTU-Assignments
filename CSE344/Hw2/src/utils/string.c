#include "utils/string.h"

int is_integer(const char *str) {
    if (*str == '\0') {
        return 0;
    }

    if (*str == '-') {
        str++;
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
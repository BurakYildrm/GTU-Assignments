#include <lib/utility.h>

using namespace myos;
using namespace myos::common;

size_t myos::strlen(const char *str) {
    size_t length = 0;
    while (*str != '\0') {
        length++;
        str++;
    }
    return length;
}

size_t myos::strlenUtf8(const char *str) {
    size_t length = 0;
    while (*str != '\0') {
        if ((*str & 0xC0) != 0x80) { // UTF-8 character
            length++;
        }
        str++;
    }
    return length;
}

void myos::itoa(int32_t value, int8_t *buffer) {
    int32_t bufferIndex = 0;
    int32_t digits = 0;
    int32_t temp = value;

    if (value < 0) {
        buffer[bufferIndex++] = '-';
        value = -value;
        temp = value;
    }

    while (temp) {
        temp /= 10;
        digits++;
    }

    if (value == 0) {
        buffer[bufferIndex++] = '0';
    } else {
        temp = value;
        int32_t tempDigits = digits;
        while (temp) {
            buffer[bufferIndex + tempDigits - 1] = '0' + temp % 10;
            temp /= 10;
            tempDigits--;
        }
        bufferIndex += digits;
    }

    buffer[bufferIndex] = '\0';
}

void myos::printf(char *str) {
    static uint16_t *VideoMemory = (uint16_t *) 0xb8000;

    static uint8_t x = 0, y = 0;

    for (int i = 0; str[i] != '\0'; ++i) {
        switch (str[i]) {
            case '\n':
                x = 0;
                y++;
                break;
            default:
                VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
                x++;
                break;
        }

        if (x >= 80) {
            x = 0;
            y++;
        }

        if (y >= 25) {
            for (y = 0; y < 25; y++)
                for (x = 0; x < 80; x++) VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}

void myos::printFormatted(char *format, void *args[]) {
    char *p = format;
    int8_t buffer[MAX_OUTPUT_LEN];
    int32_t argsIndex = 0;
    int32_t bufferIndex = 0;
    size_t formatSize = strlen(format);

    for (int32_t i = 0; i < formatSize && bufferIndex < MAX_OUTPUT_LEN - 1; i++) {
        if (p[i] == '%') {
            if (i < formatSize - 1 && p[i + 1] == 's') {
                i++;
                char *str = (char *) args[argsIndex++];

                while (*str != '\0' && bufferIndex < MAX_OUTPUT_LEN - 1) {
                    buffer[bufferIndex++] = *str++;
                }
            } else if (i < formatSize - 1 && p[i + 1] == 'd') {
                i++;
                int32_t num = *((int32_t *) args[argsIndex++]);
                int32_t digits = 0;
                int32_t temp = num;

                if (num < 0) {
                    buffer[bufferIndex++] = '-';
                    num = -num;
                    temp = num;
                }

                while (temp) {
                    temp /= 10;
                    digits++;
                }

                if (num == 0) {
                    buffer[bufferIndex++] = '0';
                } else {
                    temp = num;
                    int32_t tempDigits = digits;
                    while (temp) {
                        buffer[bufferIndex + tempDigits - 1] = '0' + temp % 10;
                        temp /= 10;
                        tempDigits--;
                    }
                    bufferIndex += digits;
                }
            } else if (i < formatSize - 1 && p[i + 1] == 'c') {
                i++;
                int8_t c = *((int8_t *) args[argsIndex++]);
                buffer[bufferIndex++] = c;
            } else if (i < formatSize - 3 && p[i + 1] == '-' && p[i + 2] == '*' && p[i + 3] == 's') {
                i += 3;
                int32_t width = *((int32_t *) args[argsIndex++]);
                char *str = (char *) args[argsIndex++];
                size_t str_length = strlenUtf8(str);

                while (*str != '\0' && bufferIndex < MAX_OUTPUT_LEN - 1) {
                    buffer[bufferIndex++] = *str++;
                }

                if (str_length < width) {
                    for (int32_t j = 0; j < width - str_length; j++) {
                        buffer[bufferIndex++] = ' ';
                    }
                }
            } else if (i < formatSize - 3 && p[i + 1] == '.' && p[i + 2] == '*' && p[i + 3] == 'c') {
                i += 3;
                int32_t width = *((int32_t *) args[argsIndex++]);
                int8_t c = *((int8_t *) args[argsIndex++]);

                for (int32_t j = 0; j < width; j++) {
                    buffer[bufferIndex++] = c;
                }
            } else if (i < formatSize - 3 && p[i + 1] == '.' && p[i + 2] == '*' && p[i + 3] == 's') {
                i += 3;
                int32_t width = *((int32_t *) args[argsIndex++]);
                char *str = (char *) args[argsIndex++];
                char *tmp = str;

                for (int32_t j = 0; j < width; j++) {
                    tmp = str;
                    while (*tmp != '\0' && bufferIndex < MAX_OUTPUT_LEN - 1) {
                        buffer[bufferIndex++] = *tmp++;
                    }
                }
            } else {
                buffer[bufferIndex++] = p[i];
            }
        } else {
            buffer[bufferIndex++] = p[i];
        }
    }

    buffer[bufferIndex] = '\0';
    printf(buffer);
}

void myos::printfHex(uint8_t key) {
    char *foo = "00";
    char *hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}

void myos::printfHex16(uint16_t key) {
    printfHex((key >> 8) & 0xFF);
    printfHex(key & 0xFF);
}

void myos::printfHex32(uint32_t key) {
    printfHex((key >> 24) & 0xFF);
    printfHex((key >> 16) & 0xFF);
    printfHex((key >> 8) & 0xFF);
    printfHex(key & 0xFF);
}

void myos::sleep(uint32_t seconds) {
    uint64_t delay = seconds * SECOND;

    for (uint64_t i = 0; i < delay; i++);
}
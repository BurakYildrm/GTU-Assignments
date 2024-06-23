#ifndef __LIB_UTILITY_H
#define __LIB_UTILITY_H

#include <common/types.h>

#define MAX_OUTPUT_LEN 256
#define MAX_INT_LEN 10
#define SECOND 100000000 // approximately 1 second

namespace myos {
    common::size_t strlen(const char *str);
    common::size_t strlenUtf8(const char *str);
    void itoa(common::int32_t value, common::int8_t *buffer);
    void printf(char *str);
    void printFormatted(char *format, void *args[]);
    void printfHex(common::uint8_t key);
    void printfHex16(common::uint16_t key);
    void printfHex32(common::uint32_t key);
    void sleep(common::uint32_t seconds);
} // namespace myos

#endif
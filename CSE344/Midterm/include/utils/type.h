#ifndef TYPE_H
#define TYPE_H

typedef enum {
    FALSE = 0,
    TRUE = 1
} Bool;

typedef enum {
    FAILURE = -1,
    SUCCESS = 0,
    END_OF_FILE = 1,
    FILE_NOT_FOUND = 2,
    LINE_NOT_FOUND = 3,
} FunctionStatus;

#endif

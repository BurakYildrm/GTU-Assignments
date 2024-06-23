#ifndef SHARED_H
#define SHARED_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils/io.h"

typedef enum {
    PLACED,
    PREPARING,
    PREPARED,
    COOKING,
    COOKED,
    DELIVERING,
    DELIVERED,
    DONE
} OrderStatus;

typedef struct {
    int x;
    int y;
} Coordinate;

typedef struct {
    Coordinate destination;
    Coordinate area_end;
    OrderStatus status;
    int id;
    int cook_id;
    int delivery_id;
    pid_t pid;
} Order;

FunctionStatus assign_signal_handler(int signo, void (*handler)(int));

#endif

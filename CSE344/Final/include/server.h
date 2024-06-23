#ifndef SERVER_H
#define SERVER_H

#include <math.h>
#include <string.h>
#include <time.h>

#include "shared.h"
#include "utils/log.h"
#include "utils/matrix.h"

#define OVEN_CAPACITY 6
#define APPARATUS_NUM 3
#define MAX_DELIVERY_AMOUNT 3

typedef struct {
    Order *entries;
    int size;
    int capacity;
} OrderQueue;

Bool is_signal_received(void);
Order dequeue(OrderQueue *order_queue);
void cleanup(void);
void *cook(void *arg);
void cook_timer_handler(union sigval sv);
void *delivery_personel(void *arg);
void deliver_order(Order order, Coordinate start, Coordinate destination);
void enqueue(OrderQueue *queue, Order order);
void manager_timer_handler(union sigval sv);
void signal_handler(int signo);
void sigterm_handler(int signo);

#endif

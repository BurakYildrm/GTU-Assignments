#ifndef MAIN_H
#define MAIN_H

#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "utils/io.h"
#include "utils/type.h"

/* parking lot limits */
#define MAX_AUTOMOBILE_SPOT 8 // maximum number of automobile parking spots
#define MAX_PICKUP_SPOT 4     // maximum number of pickup parking spots

/* maximum number of car owner and attendant threads */
#define MAX_CAR_OWNER 2 // maximum number of car owner threads
#define MAX_ATTENDANT 2 // maximum number of car attendant threads
#define MAX_CAR 12      // maximum number of cars to be generated

typedef enum {
    PARK,
    RETRIEVE
} Action;

typedef enum {
    AUTOMOBILE,
    PICKUP
} Car;

typedef struct {
    Car type;
    int id;
    int time;
} CarOwner;

void *carOwner(void *args);
void *carAttendant(void *args);
void cleanup();
void sigint_handler(int sig);

#endif

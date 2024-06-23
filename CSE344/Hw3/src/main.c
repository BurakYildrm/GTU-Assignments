#include "main.h"

Bool isFinished = FALSE;                                // flag to indicate that there are no more car owners

int mFree_automobile = MAX_AUTOMOBILE_SPOT;             // number of free automobile spots
int mFree_pickup = MAX_PICKUP_SPOT;                     // number of free pickup spots
int mLeft_cars = 0;                                     // number of cars that left because there were no spots available

CarOwner gCarOwner;                                     // global variable to store the current car owner
CarOwner parked[MAX_AUTOMOBILE_SPOT + MAX_PICKUP_SPOT]; // array to store the parked cars

sem_t mutex;                                            // limit access to global variables to one thread at a time
sem_t print_mutex;                                      // limit output to one thread at a time

sem_t newAutomobile;                                    // signal that a new automobile has arrived
sem_t newPickup;                                        // signal that a new pickup has arrived
sem_t inChangeforAutomobile;                            // signal that the car attendant for automobile is changing the automobile spots
sem_t inChangeforPickup;                                // signal that the car attendant for pickup is changing the pickup spots
sem_t retrieveCar;                                      // signal that a car owner wants to retrieve their car

pthread_t carOwners[MAX_CAR_OWNER];
pthread_t carAttendants[MAX_ATTENDANT];

int main() {
    struct sigaction sa_sigint;
    sa_sigint.sa_handler = sigint_handler;
    sigfillset(&sa_sigint.sa_mask);
    sa_sigint.sa_flags = 0;

    if (sigaction(SIGINT, &sa_sigint, NULL) == -1) {
        perror("Error setting up SIGINT handler");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    sem_init(&mutex, 0, 1);
    sem_init(&print_mutex, 0, 1);

    sem_init(&newAutomobile, 0, 0);
    sem_init(&newPickup, 0, 0);
    sem_init(&inChangeforAutomobile, 0, 0);
    sem_init(&inChangeforPickup, 0, 0);
    sem_init(&retrieveCar, 0, 0);

    Car automobile = AUTOMOBILE;
    Car pickup = PICKUP;
    Action park = PARK;
    Action retrieve = RETRIEVE;

    for (int i = 0; i < MAX_AUTOMOBILE_SPOT + MAX_PICKUP_SPOT; i++) {
        parked[i].time = 0;
    }

    pthread_create(&carAttendants[0], NULL, carAttendant, &automobile);
    pthread_create(&carAttendants[1], NULL, carAttendant, &pickup);
    pthread_create(&carOwners[0], NULL, carOwner, &park);
    pthread_create(&carOwners[1], NULL, carOwner, &retrieve);

    for (int i = 0; i < MAX_CAR_OWNER; i++) {
        pthread_join(carOwners[i], NULL);
    }

    for (int i = 0; i < MAX_ATTENDANT; i++) {
        pthread_cancel(carAttendants[i]);
        pthread_join(carAttendants[i], NULL);
    }

    cleanup();

    return 0;
}

int cmpCarOwnerByTime(const void *a, const void *b) {
    CarOwner ownerA = *((CarOwner *) a);
    CarOwner ownerB = *((CarOwner *) b);

    return ownerA.time - ownerB.time;
}

void *carOwner(void *args) {
    Action action = *((Action *) args);

    switch (action) {
        case PARK: {
            for (int i = 0; i < MAX_CAR; i++) {
                Car car = rand() % 2;

                switch (car) {
                    case AUTOMOBILE:
                        sem_wait(&mutex);
                        if (mFree_automobile == 0) {
                            my_printf("No spot available for automobile. Owner %d leaves.\n", i + 1);
                            mLeft_cars++;
                            sem_post(&mutex);
                            break;
                        }
                        sem_post(&mutex);

                        sem_wait(&mutex);
                        gCarOwner.type = AUTOMOBILE;
                        gCarOwner.id = i;
                        gCarOwner.time = rand() % 5 + 1 + i;
                        my_printf("Owner %d (automobile) arrives\n", i + 1);
                        sem_post(&mutex);
                        sem_post(&newAutomobile);
                        sem_wait(&inChangeforAutomobile);
                        sem_post(&retrieveCar);
                        break;

                    case PICKUP:
                        sem_wait(&mutex);
                        if (mFree_pickup == 0) {
                            my_printf("No spot available for pickup. Owner %d leaves.\n", i + 1);
                            mLeft_cars++;
                            sem_post(&mutex);
                            break;
                        }
                        sem_post(&mutex);

                        sem_wait(&mutex);
                        gCarOwner.type = PICKUP;
                        gCarOwner.id = i;
                        gCarOwner.time = rand() % 5 + 1 + i;
                        my_printf("Owner %d (pickup) arrives\n", i + 1);
                        sem_post(&mutex);
                        sem_post(&newPickup);
                        sem_wait(&inChangeforPickup);
                        sem_post(&retrieveCar);
                        break;
                }

                sleep(1);
            }

            sem_wait(&mutex);
            isFinished = TRUE;
            sem_post(&mutex);
            break;
        }
        case RETRIEVE: {
            int waitedTime = 0;

            while (TRUE) {
                sem_wait(&mutex);
                if (isFinished && parked[0].time == 0) {
                    sem_post(&mutex);
                    break;
                }

                sem_post(&mutex);
                sem_wait(&retrieveCar);
                sem_wait(&mutex);
                CarOwner waitingOwner = parked[0];
                int timeToSleep = waitingOwner.time - waitedTime;
                timeToSleep = timeToSleep < 0 ? 0 : timeToSleep;
                sem_post(&mutex);

                sleep(timeToSleep);

                sem_wait(&mutex);
                while (parked[0].time == waitingOwner.time && parked[0].time != 0) {
                    CarOwner owner = parked[0];

                    for (int j = 0; j < (sizeof(parked) / sizeof(CarOwner)) - 1; j++) {
                        parked[j] = parked[j + 1];
                    }

                    parked[(sizeof(parked) / sizeof(CarOwner)) - 1].time = 0;

                    switch (owner.type) {
                        case AUTOMOBILE: {
                            mFree_automobile++;
                            int time = owner.time - owner.id;
                            my_printf("Owner %d retrieved their automobile after %d second%s. Remaining automobile spots: %d\n",
                                      owner.id + 1, time, time == 1 ? "" : "s", mFree_automobile);
                            break;
                        }
                        case PICKUP: {
                            mFree_pickup++;
                            int time = owner.time - owner.id;
                            my_printf("Owner %d retrieved their pickup after %d second%s. Remaining pickup spots: %d\n", owner.id + 1, time,
                                      time == 1 ? "" : "s", mFree_pickup);
                            break;
                        }
                    }
                }

                waitedTime += timeToSleep;
                sem_post(&mutex);
            }
            break;
        }
    }

    return NULL;
}

void *carAttendant(void *args) {
    Car type = *((Car *) args);

    switch (type) {
        case AUTOMOBILE:
            while (TRUE) {
                sem_wait(&newAutomobile);
                sem_wait(&mutex);
                int index = (MAX_AUTOMOBILE_SPOT + MAX_PICKUP_SPOT) - (mFree_automobile + mFree_pickup);
                parked[index] = gCarOwner;
                qsort(parked, index + 1, sizeof(CarOwner), cmpCarOwnerByTime);
                mFree_automobile--;
                my_printf("Attendant parked the automobile of owner %d. Remaining automobile spots: %d\n", gCarOwner.id + 1,
                          mFree_automobile);
                sem_post(&mutex);
                sem_post(&inChangeforAutomobile);
            }
            break;
        case PICKUP:
            while (TRUE) {
                sem_wait(&newPickup);
                sem_wait(&mutex);
                int index = (MAX_AUTOMOBILE_SPOT + MAX_PICKUP_SPOT) - (mFree_automobile + mFree_pickup);
                parked[index] = gCarOwner;
                qsort(parked, index + 1, sizeof(CarOwner), cmpCarOwnerByTime);
                mFree_pickup--;
                my_printf("Attendant parked the pickup of owner %d. Remaining pickup spots: %d\n", gCarOwner.id + 1, mFree_pickup);
                sem_post(&mutex);
                sem_post(&inChangeforPickup);
            }
            break;
    }

    return NULL;
}

void cleanup() {
    sem_destroy(&mutex);
    sem_destroy(&newAutomobile);
    sem_destroy(&newPickup);
    sem_destroy(&inChangeforAutomobile);
    sem_destroy(&inChangeforPickup);
    sem_destroy(&retrieveCar);
}

void sigint_handler(int sig) {
    for (int i = 0; i < MAX_CAR_OWNER; i++) {
        pthread_cancel(carOwners[i]);
        pthread_join(carOwners[i], NULL);
    }

    for (int i = 0; i < MAX_ATTENDANT; i++) {
        pthread_cancel(carAttendants[i]);
        pthread_join(carAttendants[i], NULL);
    }

    cleanup();
    my_printf("\nReceived SIGINT. Exiting...\n");
    exit(EXIT_SUCCESS);
}
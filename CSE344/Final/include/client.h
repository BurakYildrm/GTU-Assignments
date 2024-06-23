#ifndef CLIENT_H
#define CLIENT_H

#include "shared.h"

Order generate_order(int p, int q);
void cleanup(void);
void *client(void *arg);
void sigint_handler(int signo);
void sigpipe_handler(int signo);

#endif

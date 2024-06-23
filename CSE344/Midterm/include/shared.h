#ifndef SHARED_H
#define SHARED_H

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "utils/io.h"
#include "utils/log.h"
#include "utils/string.h"
#include "utils/type.h"

/* Definitions of semaphores */
#define SEM_CONN_REQUEST_NAME_FORMAT "/sem_%ld_conn_request" // connection requests
#define SEM_CONN_REQUEST_NAME_LEN 64
#define SEM_CONN_RESPONSE_NAME_FORMAT "/sem_%ld_conn_response" // connection responses
#define SEM_CONN_RESPONSE_NAME_LEN 64
#define SEM_COMM_REQUEST_NAME_FORMAT "/sem_%ld_comm_request" // general communication requests
#define SEM_COMM_REQUEST_NAME_LEN 64
#define SEM_COMM_RESPONSE_NAME_FORMAT "/sem_%ld_comm_response" // general communication responses
#define SEM_COMM_RESPONSE_NAME_LEN 64
#define SEM_WAIT_TERM_REQUEST_NAME_FORMAT                                                          \
    "/sem_%ld_wait_term_request" // waiting client termination requests
#define SEM_WAIT_TERM_REQUEST_NAME_LEN 64
#define SEM_WAIT_TERM_RESPONSE_NAME_FORMAT                                                         \
    "/sem_%ld_wait_term_response" // waiting client termination responses
#define SEM_WAIT_TERM_RESPONSE_NAME_LEN 64

/* Definitions of shared memories */
#define SHM_CONN_NAME_FORMAT "/shm_%ld_conn" // Shared memory for connection data
#define SHM_CONN_NAME_LEN 64
#define SHM_CONN_SIZE sizeof(ConnectionRequest)
#define SHM_COMM_NAME_FORMAT "/shm_%ld_comm" // Shared memory for general communication data
#define SHM_COMM_NAME_LEN 64
#define SHM_COMM_SIZE sizeof(CommunicationData)
#define SHM_WAIT_TERM_NAME_FORMAT                                                                  \
    "/shm_%ld_wait_term" // Shared memory for waiting client termination
#define SHM_WAIT_TERM_NAME_LEN 64
#define SHM_WAIT_TERM_SIZE sizeof(pid_t)

/* Definitions of semaphore, shared memory and directory permissions */
#define SEM_PERMS 0666
#define SHM_PERMS 0777
#define DIR_PERMS 0777

/* Definitions of command and argument numbers */
#define COMMAND_NUM 9
#define MAX_ARGS 3

typedef enum {
    HELP = 0,
    LIST = 1,
    READ = 2,
    WRITE = 3,
    UPLOAD = 4,
    DOWNLOAD = 5,
    ARCHIVE = 6,
    KILL_SERVER = 7,
    QUIT = 8,
    UNKNOWN_COMMAND = -1,
} Command;

typedef enum {
    CONNECT = 0,
    TRY_CONNECT = 1,
    UNKNOWN_CONNECTION = -1,
} ConnectionOption;

typedef enum {
    RESPONSE_OK = 0,
    RESPONSE_CONTINUE = 2,
    RESPONSE_NOT_FOUND = 3,
} ResponseStatus;

typedef struct {
    int args_num;
    char args[MAX_ARGS][NAME_MAX + 1];
    Command command;
} Request;

typedef struct {
    char filename[NAME_MAX + 1];
    pid_t pid;
    ResponseStatus status;
} Response;

typedef struct {
    int bytes;
    char buffer[MAX_OUTPUT_LEN];
    Request request;
    Response response;
} CommunicationData;

typedef struct {
    pid_t pid;
    ConnectionOption option;
} ConnectionRequest;

extern const int command_args[COMMAND_NUM];
extern const char *command_names[COMMAND_NUM];

FunctionStatus assign_signal_handler(int signo, void (*handler)(int));

#endif

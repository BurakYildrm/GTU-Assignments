#ifndef SERVER_H
#define SERVER_H

#include "shared.h"

#define DIR_LOCK_NAME_FORMAT ".%s.dirlock"
#define DIR_LOCK_NAME_LEN (NAME_MAX + 10)
#define TEMP_FILE_FORMAT "temp_%ld.txt"

typedef enum {
    RUNNING = 0,
    WAITING = 1
} ProcessState;

typedef struct {
    int id;
    pid_t server_pid;
    pid_t client_pid;
    ProcessState state;
} ProcessTableEntry;

typedef struct {
    int size;
    int num_running;
    int num_waiting;
    int process_count;
    ProcessTableEntry *entries;
} ProcessTable;

FunctionStatus send_file_content(const char *filename, int line_num, Bool is_archive);
FunctionStatus write_content_to_file(const char *filename, char *content, int line_num);
int add_process(ProcessTable *table, pid_t server_pid, pid_t client_pid);
int cmp_entry_by_client_pid(const void *entry, const void *client_pid);
int cmp_entry_by_server_pid(const void *entry, const void *server_pid);
void archive();
void child_server();
void cleanup();
void cleanup_child();
void cleanup_parent();
void download(char args[MAX_ARGS][NAME_MAX + 1]);
void error_exit(const char *format, ...);
void kill_server();
void list();
void process_command(Command command, char args[MAX_ARGS][NAME_MAX + 1], int args_num);
void quit();
void readF(char args[MAX_ARGS][NAME_MAX + 1], int args_num);
void sigchld_handler(int sig);
void sigint_handler(int sig);
void sigterm_handler(int sig);
void sigusr1_handler(int sig);
void sigusr2_handler(int sig);
void unescape_newlines(char *str);
void upload();
void writeT(char args[MAX_ARGS][NAME_MAX + 1], int args_num);

#endif

#ifndef CLIENT_H
#define CLIENT_H

#include "shared.h"

Command get_command(const char *command);
ConnectionOption get_connection_action(const char *action);
FunctionStatus clean_archive_dir();
void archive(char filename[NAME_MAX + 1]);
void cleanup();
void dispatch_command(Command command, char args[MAX_ARGS][NAME_MAX + 1], int args_num);
void download(char filename[NAME_MAX + 1]);
void error_exit(const char *format, ...);
void kill_server();
void list();
void print_all_commands();
void print_command_help(Command command);
void quit();
void readF(char args[MAX_ARGS][NAME_MAX + 1], int args_num);
void sigint_handler(int sig);
void sigterm_handler(int sig);
void sigusr1_handler(int sig);
void sigusr2_handler(int sig);
void upload(char filename[NAME_MAX + 1]);
void writeT(char args[MAX_ARGS][NAME_MAX + 1], int args_num);

#endif

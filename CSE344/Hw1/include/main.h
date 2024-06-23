#ifndef MAIN_H
#define MAIN_H

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils/array.h"
#include "utils/io.h"
#include "utils/log.h"
#include "utils/memory.h"
#include "utils/string.h"

#define ENTRY_PER_PAGE 5
#define MAX_ARGS 3
#define CMDS_NUM 8
#define TERM_SIGS_NUM 6

#define CORNER_LEFT_DOWN "\u2570"
#define CORNER_LEFT_UP "\u256D"
#define CORNER_RIGHT_DOWN "\u256F"
#define CORNER_RIGHT_UP "\u256E"
#define CROSS "\u253C"
#define HORIZONTAL_LINE "\u2500"
#define T_DOWN "\u2534"
#define T_LEFT "\u251C"
#define T_RIGHT "\u2524"
#define T_UP "\u252C"
#define VERTICAL_LINE "\u2502"

#define MY_CLOSE_1(fd) my_close((fd), NULL)
#define MY_CLOSE_2(fd, filename) my_close((fd), (filename))
#define GET_MY_CLOSE(_1, _2, NAME, ...) NAME
#define my_close(...) GET_MY_CLOSE(__VA_ARGS__, MY_CLOSE_2, MY_CLOSE_1)(__VA_ARGS__)

#define MY_FGETS_3(buffer, size, fd) my_fgets(buffer, size, fd, NULL)
#define MY_FGETS_4(buffer, size, fd, filename) my_fgets(buffer, size, fd, filename)
#define GET_MY_FGETS(_1, _2, _3, _4, NAME, ...) NAME
#define my_fgets(...) GET_MY_FGETS(__VA_ARGS__, MY_FGETS_4, MY_FGETS_3)(__VA_ARGS__)

typedef struct {
    const char *name;
    int args_num;
} Command;

typedef struct {
    const char *fullname;
    const char *grade;
} Student;

int cmp_student_by_grade_asc(const void *student1, const void *student2);
int cmp_student_by_grade_desc(const void *student1, const void *student2);
int cmp_student_by_name_asc(const void *student1, const void *student2);
int cmp_student_by_name_desc(const void *student1, const void *student2);
int cmp_student_to_name(const void *student, const void *name);
int *get_cmd_args_num(const char *command, const Command *commands, size_t size);
int is_valid_cmd(const char *command, const Command *commands, size_t size);
int read_file(int fd, const char *filename, Student **buffer, int init_size, int *longest_name,
              int start_line, int end_line);
void add_student(const char *fullname, const char *grade, const char *filename);
void cleanup();
void create_file(const char *filename);
void dispatch_cmd(const char *command, int args_num, char **args);
void list_grades(const char *filename);
void list_some(int entry_num, int page_num, const char *filename);
void print_all_cmds(const Command *commands, size_t size);
void print_cmd(const char *command);
void print_students(const Student *students, int size, int longest_name);
void reset_globals();
void search_student(const char *fullname, const char *filename);
void show_all(const char *filename);
void sort_all(const char *field, int order, const char *filename);
void term_sigs_handler(int signo);

#endif

#include "main.h"

int g_fd = INT_MIN;
int g_student_buffer_len = 0;
pid_t pid = -1;
Student *g_student_buffer = NULL;
volatile sig_atomic_t signal_flag = 0;

const Command commands[CMDS_NUM] = {
    {"gtuStudentGrades", 0}, {"gtuStudentGrades", 1}, {"addStudentGrade", 3}, {"searchStudent", 2},
    {"sortAll", 3},          {"showAll", 1},          {"listGrades", 1},      {"listSome", 3}};

int main() {
    const int term_sigs[TERM_SIGS_NUM] = {SIGBUS, SIGHUP, SIGINT, SIGQUIT, SIGSEGV, SIGTERM};

    char input[MAX_INPUT];
    char *command;
    char *args[MAX_ARGS];

    int args_counter;
    int while_counter = 0;

    int status;

    struct sigaction sa;
    sa.sa_handler = term_sigs_handler;
    sa.sa_flags = 0;
    sigfillset(&sa.sa_mask);

    for (int i = 0; i < TERM_SIGS_NUM; i++) {
        if (sigaction(term_sigs[i], &sa, NULL) == -1) {
            log_message("Failed to register the signal handler for %s.\n",
                        my_strsignal(term_sigs[i]));
            perror("Error registering signal handler");
            return -1;
        }
    }

    if (atexit(cleanup) != 0) {
        perror("Error registering cleanup function");
        return -1;
    }

    init_log();
    log_message("[%s] Program started with PID %ld.\n", get_timestamp("%T"), (long) getpid());
    my_printf("Enter (q) to quit.\n");

    while (1) {
        my_printf("%s> Enter a command: ", while_counter != 0 ? "\n" : "");
        my_fgets(input, sizeof(input), STDIN_FILENO);

        char *token = my_strtok(input, '\"');
        command = trim(token);
        args_counter = 0;

        if (command == NULL) {
            while_counter++;
            continue;
        }

        if (my_strcmp(command, "q") == 0) {
            log_message("[%s] Received user termination request. Initiating shutdown.\n",
                        get_timestamp("%T"));
            exit(EXIT_SUCCESS);
        }

        if (!is_valid_cmd(command, commands, CMDS_NUM)) {
            my_printf("Invalid command. Use gtuStudentGrades to display "
                      "all available commands.\n");
            while_counter++;
            continue;
        }

        while ((token = my_strtok(NULL, '\"')) != NULL && args_counter < MAX_ARGS) {
            args[args_counter++] = trim(token);
        }

        int *args_num = get_cmd_args_num(command, commands, CMDS_NUM);

        if ((args_counter == MAX_ARGS && token != NULL) ||
            (args_counter != args_num[0] && args_counter != args_num[1])) {
            my_printf("Invalid number of arguments. Here are the possible "
                      "options for the command:\n");
            print_cmd(command);
            while_counter++;
            continue;
        }

        switch ((pid = fork())) {
        case -1:
            log_message("[%s] Failed to create a child process.\n", get_timestamp("%T"));
            perror("Error creating child process");
            exit(EXIT_FAILURE);
            break;
        case 0:
            dispatch_cmd(command, args_counter, args);
            break;
        default:
            break;
        }

        waitpid(pid, &status, 0);
        log_message("[%s] Child process with PID %ld terminated with status %d.\n",
                    get_timestamp("%T"), (long) pid, WEXITSTATUS(status));

        pid = -1;
        while_counter++;
    }

    return 0;
}

int cmp_student_by_grade_asc(const void *student1, const void *student2) {
    const Student *student1_ = (const Student *) student1;
    const Student *student2_ = (const Student *) student2;
    return my_strcmp(student1_->grade, student2_->grade);
}

int cmp_student_by_grade_desc(const void *student1, const void *student2) {
    const Student *student1_ = (const Student *) student1;
    const Student *student2_ = (const Student *) student2;
    return my_strcmp(student2_->grade, student1_->grade);
}

int cmp_student_by_name_asc(const void *student1, const void *student2) {
    const Student *student1_ = (const Student *) student1;
    const Student *student2_ = (const Student *) student2;
    return my_strcmp(student1_->fullname, student2_->fullname);
}

int cmp_student_by_name_desc(const void *student1, const void *student2) {
    const Student *student1_ = (const Student *) student1;
    const Student *student2_ = (const Student *) student2;
    return my_strcmp(student2_->fullname, student1_->fullname);
}

int cmp_student_to_name(const void *student, const void *name) {
    const Student *student_ = (const Student *) student;
    const char *name_ = (const char *) name;
    return my_strcmp(student_->fullname, name);
}

int *get_cmd_args_num(const char *command, const Command *commands, size_t size) {
    static int args_num[] = {-1, -1};
    int count = 0;

    for (int i = 0; i < size; i++) {
        if (my_strcmp(command, commands[i].name) == 0) {
            args_num[count++] = commands[i].args_num;
        }
    }

    return args_num;
}

int is_valid_cmd(const char *command, const Command *commands, size_t size) {
    for (int i = 0; i < size; i++) {
        if (my_strcmp(command, commands[i].name) == 0) {
            return 1;
        }
    }
    return 0;
}

int read_file(int fd, const char *filename, Student **buffer, int init_size, int *longest_name,
              int start_line, int end_line) {
    char line[MAX_OUTPUT];
    int i = 0;
    int lines_read = 0;
    int size = init_size;
    int upper_limit = end_line == -1 ? INT_MAX : end_line;

    while (lines_read < upper_limit && my_fgets(line, sizeof(line), fd, filename) != NULL) {
        if (lines_read++ < start_line) {
            continue;
        }

        char *fullname = my_strtok(line, ',');
        char *grade = my_strtok(NULL, ',');
        int fullname_len = my_strlen_utf8(fullname);

        if (longest_name != NULL && fullname_len > *longest_name) {
            *longest_name = fullname_len;
        }

        if (i == size) {
            size *= 2;
            *buffer = (Student *) my_realloc(*buffer, size * sizeof(Student));
        }

        (*buffer)[i].fullname = my_strdup(fullname);
        (*buffer)[i].grade = my_strdup(grade);
        g_student_buffer_len = ++i;
    }

    return i;
}

void add_student(const char *fullname, const char *grade, const char *filename) {
    int init_size = 10;
    int line_num = 0;
    int student_index = 0;

    if (!file_exists(filename)) {
        log_message("[%s] File \"%s\" not found.\n", get_timestamp("%T"), filename);
        my_printf("File not found.\n");
        return;
    }

    g_student_buffer = (Student *) my_malloc(init_size * sizeof(Student));
    g_fd = my_open(filename, READ_ONLY, FILE_PERMS);
    line_num = read_file(g_fd, filename, &g_student_buffer, init_size, NULL, 0, -1);
    student_index =
        indexof_struct(g_student_buffer, line_num, sizeof(Student), fullname, cmp_student_to_name);
    my_close(g_fd, filename);

    if (student_index != -1) {
        g_fd = my_open(filename, OVERWRITE, FILE_PERMS);

        free(g_student_buffer[student_index].grade);
        g_student_buffer[student_index].grade = my_strdup(grade);

        for (int i = 0; i < line_num; i++) {
            if (my_fprintf(g_fd, "%s, %s\n", g_student_buffer[i].fullname,
                           g_student_buffer[i].grade) == -1) {
                log_message("[%s] Failed to write to the file \"%s\" in the child "
                            "process with PID %ld.\n",
                            get_timestamp("%T"), filename, (long) getpid());
                perror("Error writing to file");
                exit(EXIT_FAILURE);
            }
        }

        log_message("[%s] Updated the grade of the student \"%s\" to \"%s\" in the file \"%s\" "
                    "in the child process with PID %ld.\n",
                    get_timestamp("%T"), fullname, grade, filename, (long) getpid());
        my_printf("Student updated successfully.\n");

    } else {
        g_fd = my_open(filename, WRITE_ONLY, FILE_PERMS);

        if (my_fprintf(g_fd, "%s, %s\n", fullname, grade) == -1) {
            log_message("[%s] Failed to write to the file \"%s\" in the child "
                        "process with PID %ld.\n",
                        get_timestamp("%T"), filename, (long) getpid());
            perror("Error writing to file");
            exit(EXIT_FAILURE);
        }

        log_message(
            "[%s] Added the student \"%s\" with grade \"%s\" to the file \"%s\" in the child "
            "process with PID %ld.\n",
            get_timestamp("%T"), fullname, grade, filename, (long) getpid());
        my_printf("Student added successfully.\n");
    }

    my_close(g_fd, filename);
    reset_globals();
}

void cleanup() {
    if (pid != 0) {
        if (signal_flag != 0) {
            log_message("[%s] Encountered %s. Initiating shutdown.\n", get_timestamp("%T"),
                        my_strsignal(signal_flag));
        }
        log_message("[%s] Program terminated.\n", get_timestamp("%T"));
        my_printf("\nProgram terminated.\n");
    } else {
        if (g_fd != INT_MIN && g_fd != -1) {
            if (close(g_fd) == -1) {
                log_message("[%s] Failed to close the file in the child "
                            "process with PID %ld.\n",
                            get_timestamp("%T"), (long) getpid());
                perror("Error closing file");
            }
        }

        if (g_student_buffer != NULL) {
            for (int i = 0; i < g_student_buffer_len; i++) {
                free(g_student_buffer[i].fullname);
                free(g_student_buffer[i].grade);
            }
            free(g_student_buffer);
        }
    }
}

void create_file(const char *filename) {
    if (file_exists(filename)) {
        log_message("[%s] File \"%s\" already exists.\n", get_timestamp("%T"), filename);
        my_printf("File already exists.\n");
        return;
    }

    g_fd = my_open(filename, READ_WRITE, FILE_PERMS);

    log_message("[%s] Created the file \"%s\" in the child process "
                "with PID %ld.\n",
                get_timestamp("%T"), filename, (long) getpid());
    my_printf("File created successfully.\n");
    my_close(g_fd, filename);

    g_fd = INT_MIN;
}

void dispatch_cmd(const char *command, int args_num, char **args) {
    log_message("[%s] Created a child process with PID %ld for the command \"%s",
                get_timestamp("%T"), (long) getpid(), command);
    for (int i = 0; i < args_num; i++) {
        log_message(" '%s'", args[i]);
    }
    log_message("\".\n");

    if (my_strcmp(command, "gtuStudentGrades") == 0) {
        args_num == 0 ? print_all_cmds(commands, CMDS_NUM) : create_file(args[0]);
    } else if (my_strcmp(command, "addStudentGrade") == 0) {
        add_student(args[0], args[1], args[2]);
    } else if (my_strcmp(command, "searchStudent") == 0) {
        search_student(args[0], args[1]);
    } else if (my_strcmp(command, "sortAll") == 0) {
        if (my_strcmp(args[0], "name") != 0 && my_strcmp(args[0], "grade") != 0) {
            my_printf("Invalid argument. Please enter either \"name\" or \"grade\" as the first "
                      "argument.\n");
        } else if (my_strcmp(args[1], "asc") != 0 && my_strcmp(args[1], "desc") != 0) {
            my_printf("Invalid argument. Please enter either \"asc\" or \"desc\" as the second "
                      "argument.\n");
        } else {
            sort_all(args[0], my_strcmp(args[1], "asc") == 0 ? 0 : 1, args[2]);
        }
    } else if (my_strcmp(command, "showAll") == 0) {
        show_all(args[0]);
    } else if (my_strcmp(command, "listGrades") == 0) {
        list_grades(args[0]);
    } else if (my_strcmp(command, "listSome") == 0) {
        int entry_num = parse_int(args[0]);
        int page_num = parse_int(args[1]);

        if (entry_num == INT_MIN || page_num == INT_MIN) {
            my_printf("Invalid arguments. Please enter valid numbers.\n");
        } else if (entry_num < 1 || page_num < 1) {
            my_printf("Invalid arguments. Please enter positive numbers.\n");
        } else {
            list_some(entry_num, page_num, args[2]);
        }
    }
    exit(EXIT_SUCCESS);
}

void list_grades(const char *filename) {
    int init_size = 5;
    int line_num = 0;
    int longest_name = 0;

    if (!file_exists(filename)) {
        log_message("[%s] File \"%s\" not found.\n", get_timestamp("%T"), filename);
        my_printf("File not found.\n");
        return;
    }

    g_student_buffer = (Student *) my_malloc(init_size * sizeof(Student));
    g_fd = my_open(filename, READ_ONLY, FILE_PERMS);
    line_num = read_file(g_fd, filename, &g_student_buffer, init_size, &longest_name, 0, 5);

    print_students(g_student_buffer, line_num, longest_name);
    log_message(
        "[%s] Displayed the first %d student entries from the file \"%s\" in the child process "
        "with PID %ld.\n",
        get_timestamp("%T"), line_num, filename, (long) getpid());
    my_close(g_fd, filename);
    reset_globals();
}

void list_some(int entry_num, int page_num, const char *filename) {
    int init_size = 10;
    int line_num = 0;
    int longest_name = 0;
    int start_line = (page_num - 1) * ENTRY_PER_PAGE;
    int end_line = start_line + entry_num;

    if (!file_exists(filename)) {
        log_message("[%s] File \"%s\" not found.\n", get_timestamp("%T"), filename);
        my_printf("File not found.\n");
        return;
    }

    g_student_buffer = (Student *) my_malloc(init_size * sizeof(Student));
    g_fd = my_open(filename, READ_ONLY, FILE_PERMS);
    line_num = read_file(g_fd, filename, &g_student_buffer, init_size, &longest_name, start_line,
                         end_line);

    print_students(g_student_buffer, line_num, longest_name);

    if (start_line + 1 == end_line) {
        log_message("[%s] Displayed the %d%s student entry from the file \"%s\" in the "
                    "child process with PID %ld.\n",
                    get_timestamp("%T"), start_line + 1, ordinal_suffix(start_line + 1), filename,
                    (long) getpid());
    } else {
        log_message(
            "[%s] Displayed the student entries from %d%s to %d%s from the file \"%s\" in the "
            "child process with PID %ld.\n",
            get_timestamp("%T"), start_line + 1, ordinal_suffix(start_line + 1), end_line,
            ordinal_suffix(end_line), filename, (long) getpid());
    }

    my_close(g_fd, filename);
    reset_globals();
}

void print_all_cmds(const Command *commands, size_t size) {
    char *command_names[CMDS_NUM];

    for (int i = 0; i < size; i++) {
        command_names[i] = commands[i].name;
    }

    int unique_cmds_num = rm_dups_str(command_names, size);

    log_message("[%s] Displayed all available commands in the child process with PID %ld.\n",
                get_timestamp("%T"), (long) getpid());

    my_printf("\nNote: Please ensure all arguments are enclosed in double quotes (\"\").\n\nHere "
              "are the possible commands:\n");
    for (int i = 0; i < unique_cmds_num; i++) {
        print_cmd(command_names[i]);
    }
}

void print_cmd(const char *command) {
    if (my_strcmp(command, "gtuStudentGrades") == 0) {
        my_printf("  gtuStudentGrades: Displays a list of all available "
                  "commands and their descriptions.\n"
                  "  gtuStudentGrades <filename>: Creates a new file with "
                  "the specified filename for storing student grades.\n");
    } else if (my_strcmp(command, "addStudentGrade") == 0) {
        my_printf("  addStudentGrade <fullname> <grade> <filename>: "
                  "Appends the specified student's full name and grade to "
                  "the designated file.\n");
    } else if (my_strcmp(command, "searchStudent") == 0) {
        my_printf("  searchStudent <fullname> <filename>: Searches "
                  "for and displays the grade of a student with the given "
                  "full name in the specified file.\n");
    } else if (my_strcmp(command, "sortAll") == 0) {
        my_printf("  sortAll <field> <order> <filename>: Sorts and displays all student "
                  "entries by the specified field in the specified order from the specified "
                  "file.\n");
    } else if (my_strcmp(command, "showAll") == 0) {
        my_printf("  showAll <filename>: Displays all student entries "
                  "stored in the specified file.\n");
    } else if (my_strcmp(command, "listGrades") == 0) {
        my_printf("  listGrades <filename>: Displays the first five student "
                  "entries from the specified file.\n");
    } else if (my_strcmp(command, "listSome") == 0) {
        my_printf("  listSome <numOfEntries> <pageNumber> <filename>: "
                  "Lists a specific range of student entries based on the "
                  "provided number of entries and page number from the "
                  "specified file.\n");
    }
}

void print_students(const Student *students, int size, int longest_name) {
    if (size == 0) {
        my_printf("No students found.\n");
        return;
    }

    my_printf("\n%s%.*s%s%.*s%s\n", CORNER_LEFT_UP, longest_name + 1, HORIZONTAL_LINE, T_UP, 5,
              HORIZONTAL_LINE, CORNER_RIGHT_UP);
    my_printf("%s%-*s%sGrade%s\n", VERTICAL_LINE, longest_name + 1, "Full Name", VERTICAL_LINE,
              VERTICAL_LINE);
    my_printf("%s%.*s%s%.*s%s\n", T_LEFT, longest_name + 1, HORIZONTAL_LINE, CROSS, 5,
              HORIZONTAL_LINE, T_RIGHT);
    for (int i = 0; i < size; i++) {
        my_printf("%s%-*s%s%-*s%s\n", VERTICAL_LINE, longest_name + 1, students[i].fullname,
                  VERTICAL_LINE, 5, students[i].grade, VERTICAL_LINE);
    }
    my_printf("%s%.*s%s%.*s%s\n", CORNER_LEFT_DOWN, longest_name + 1, HORIZONTAL_LINE, T_DOWN, 5,
              HORIZONTAL_LINE, CORNER_RIGHT_DOWN);
}

void reset_globals() {
    for (int i = 0; i < g_student_buffer_len; i++) {
        free(g_student_buffer[i].fullname);
        free(g_student_buffer[i].grade);
    }
    free(g_student_buffer);

    g_fd = INT_MIN;
    g_student_buffer = NULL;
    g_student_buffer_len = 0;
}

void search_student(const char *fullname, const char *filename) {
    char line[MAX_OUTPUT];
    int is_found = 0;

    if (!file_exists(filename)) {
        log_message("[%s] File \"%s\" not found.\n", get_timestamp("%T"), filename);
        my_printf("File not found.\n");
        return;
    }

    g_fd = my_open(filename, READ_ONLY, FILE_PERMS);

    while (my_fgets(line, sizeof(line), g_fd, filename) != NULL) {
        char *line_fullname = my_strtok(line, ',');
        char *grade = my_strtok(NULL, ',');

        if (my_strcmp(line_fullname, fullname) == 0) {
            my_printf("Grade of %s is %s.\n", fullname, grade);
            is_found = 1;
            break;
        }
    }

    if (!is_found) {
        my_printf("Student not found.\n");
    }

    log_message("[%s] Searched for the student \"%s\" in the file \"%s\" in the child "
                "process with PID %ld.\n",
                get_timestamp("%T"), fullname, filename, (long) getpid());
    my_close(g_fd, filename);

    g_fd = INT_MIN;
}

void show_all(const char *filename) {
    int init_size = 10;
    int line_num = 0;
    int longest_name = 0;

    if (!file_exists(filename)) {
        log_message("[%s] File \"%s\" not found.\n", get_timestamp("%T"), filename);
        my_printf("File not found.\n");
        return;
    }

    g_student_buffer = (Student *) my_malloc(init_size * sizeof(Student));
    g_fd = my_open(filename, READ_ONLY, FILE_PERMS);
    line_num = read_file(g_fd, filename, &g_student_buffer, init_size, &longest_name, 0, -1);

    print_students(g_student_buffer, line_num, longest_name);
    log_message("[%s] Displayed all student entries from the file \"%s\" in the child process "
                "with PID %ld.\n",
                get_timestamp("%T"), filename, (long) getpid());
    my_close(g_fd, filename);
    reset_globals();
}

void sort_all(const char *field, int order, const char *filename) {
    int init_size = 10;
    int line_num = 0;
    int longest_name = 0;
    int (*cmp)(const void *, const void *) =
        my_strcmp(field, "name") == 0
            ? (order == 0 ? cmp_student_by_name_asc : cmp_student_by_name_desc)
            : (order == 0 ? cmp_student_by_grade_asc : cmp_student_by_grade_desc);

    if (!file_exists(filename)) {
        log_message("[%s] File \"%s\" not found.\n", get_timestamp("%T"), filename);
        my_printf("File not found.\n");
        return;
    }

    g_student_buffer = (Student *) my_malloc(init_size * sizeof(Student));
    g_fd = my_open(filename, READ_ONLY, FILE_PERMS);
    line_num = read_file(g_fd, filename, &g_student_buffer, init_size, &longest_name, 0, -1);

    qsort(g_student_buffer, line_num, sizeof(Student), cmp);
    print_students(g_student_buffer, line_num, longest_name);
    log_message("[%s] Sorted all student entries from the file \"%s\" in the child process "
                "with PID %ld.\n",
                get_timestamp("%T"), filename, (long) getpid());
    my_close(g_fd, filename);
    reset_globals();
}

void term_sigs_handler(int signo) {
    signal_flag = signo;
    exit(EXIT_SUCCESS);
}
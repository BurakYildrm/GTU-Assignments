#include "main.h"

bool is_parent = false;
bool is_transmission_complete = false;
pid_t child1_pid = -1;
pid_t child2_pid = -1;
extern pid_t parent_pid;
volatile sig_atomic_t child_counter = 0;

int main() {
    parent_pid = getpid();

    const int term_sigs[TERM_SIGS_NUM] = {SIGBUS, SIGHUP, SIGINT, SIGQUIT, SIGSEGV, SIGTERM};
    int number;

    char command[] = "multiply";
    char input[MAX_INPUT_LEN];

    FifoData data;
    memset(&data, 0, sizeof(data));

    struct sigaction sa_term;
    sa_term.sa_handler = term_sigs_handler;
    sa_term.sa_flags = 0;
    sigfillset(&sa_term.sa_mask);

    for (int i = 0; i < TERM_SIGS_NUM; i++) {
        if (sigaction(term_sigs[i], &sa_term, NULL) == -1) {
            perror("Error registering termination signals handler");
            return -1;
        }
    }

    struct sigaction sa_chld;
    sa_chld.sa_handler = sigchld_handler;
    sa_chld.sa_flags = 0;
    sigfillset(&sa_chld.sa_mask);

    if (sigaction(SIGCHLD, &sa_chld, NULL) == -1) {
        perror("Error registering SIGCHLD handler");
        return -1;
    }

    struct sigaction sa_usr1;
    sa_usr1.sa_handler = sigusr1_handler;
    sa_usr1.sa_flags = 0;
    sigfillset(&sa_usr1.sa_mask);

    if (sigaction(SIGUSR1, &sa_usr1, NULL) == -1) {
        perror("Error registering SIGUSR1 handler");
        return -1;
    }

    struct sigaction sa_usr2;
    sa_usr2.sa_handler = sigusr2_handler;
    sa_usr2.sa_flags = 0;
    sigfillset(&sa_usr2.sa_mask);

    if (sigaction(SIGUSR2, &sa_usr2, NULL) == -1) {
        perror("Error registering SIGUSR2 handler");
        return -1;
    }

    if (atexit(cleanup) != 0) {
        perror("Error registering cleanup function");
        return -1;
    }

    while (1) {
        my_printf("Enter a number: ");
        my_fgets(input, sizeof(input), STDIN_FILENO);

        if (!is_integer(input)) {
            my_printf("Invalid input. Please enter a valid number.\n\n");
            continue;
        }

        number = parse_int(input);
        if (number < 1 || number > MAX_INPUT_VALUE) {
            my_printf("Invalid input. Please enter a number between 1 and %d.\n\n",
                      MAX_INPUT_VALUE);
            continue;
        }

        break;
    }

    srand(time(NULL));
    my_mkfifo("fifo_1", FIFO_PERMS);
    my_mkfifo("fifo_2", FIFO_PERMS);

    switch ((child1_pid = fork())) {
    case -1:
        perror("Error creating child 1");
        exit(EXIT_FAILURE);
        break;
    case 0:
        my_printf("Child 1 with PID %ld is started.\n", (long) getpid());
        child_1();
        break;
    default:
        break;
    }

    switch ((child2_pid = fork())) {
    case -1:
        perror("Error creating child 2");
        kill(child1_pid, SIGUSR2);
        exit(EXIT_FAILURE);
        break;
    case 0:
        my_printf("Child 2 with PID %ld is started.\n", (long) getpid());
        child_2();
        break;
    default:
        is_parent = true;
        break;
    }

    data.length = number;
    for (int i = 0; i < number; i++) {
        data.value[i] = rand() % 100;
    }

    int fifo_1 = my_open("fifo_1", O_WRONLY, FILE_PERMS);
    my_write(fifo_1, &data, sizeof(data));
    my_close(fifo_1);

    int fifo_2 = my_open("fifo_2", O_WRONLY, FILE_PERMS);
    my_write(fifo_2, command, sizeof(command));
    my_write(fifo_2, &data, sizeof(data));
    my_close(fifo_2);

    is_transmission_complete = true;

    my_printf("Numbers: ");
    for (int i = 0; i < number; i++) {
        my_printf("%d%s", data.value[i], i == number - 1 ? "\n" : ", ");
    }

    while (1) {
        my_printf("proceeding\n");
        sleep(2);
    }

    return 0;
}

void child_1() {
    int fd = my_open("fifo_1", O_RDONLY, FILE_PERMS);
    int result = 0;
    FifoData data;

    my_read(fd, &data, sizeof(data));
    my_close(fd);
    sleep(SLEEP_TIME);

    for (int i = 0; i < data.length; i++) {
        result += data.value[i];
    }

    fd = my_open("fifo_2", O_WRONLY, FILE_PERMS);
    my_write(fd, &result, sizeof(result));
    my_close(fd);

    exit(EXIT_SUCCESS);
}

void child_2() {
    char command[CMD_SIZE];
    int fd = my_open("fifo_2", O_RDONLY, FILE_PERMS);
    long multiply = 1;
    int result;
    FifoData data;

    my_read(fd, command, sizeof(command));
    if (my_strcmp(command, "multiply") == 0) {
        my_read(fd, &data, sizeof(data));
    }
    my_close(fd);
    sleep(SLEEP_TIME);

    for (int i = 0; i < data.length; i++) {
        multiply *= data.value[i];
    }

    fd = my_open("fifo_2", O_RDONLY, FILE_PERMS);
    my_read(fd, &result, sizeof(result));
    my_close(fd);

    my_printf("Result: %ld\n", result + multiply);
    exit(EXIT_SUCCESS);
}

void cleanup() {
    if (is_parent) {
        unlink("fifo_1");
        unlink("fifo_2");
    }
}

void sigchld_handler(int signo) {
    pid_t pid;
    int status;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        my_printf("Child with PID %ld has terminated with status %d.\n", (long) pid,
                  WEXITSTATUS(status));

        if (++child_counter == 2) {
            exit(EXIT_SUCCESS);
        }
    }
}

void sigusr1_handler(int signo) {
    my_printf(
        "Child with PID %ld has terminated with status %d due to incomplete data transmission.\n",
        (long) getpid(), EXIT_SUCCESS);
    exit(EXIT_SUCCESS);
}

void sigusr2_handler(int signo) {
    if (is_parent) {
        if (child1_pid != -1) {
            kill(child1_pid, SIGUSR2);
        }
        if (child2_pid != -1) {
            kill(child2_pid, SIGUSR2);
        }
        exit(EXIT_SUCCESS);
    } else {
        if (getpid() != parent_pid) {
            my_printf(
                "Child with PID %ld has terminated abnormally with status %d due to an error.\n",
                (long) getpid(), EXIT_FAILURE);
        }
        exit(EXIT_FAILURE);
    }
}

void term_sigs_handler(int signo) {
    if (is_parent) {
        if (!is_transmission_complete) {
            if (child1_pid != -1) {
                kill(child1_pid, SIGUSR1);
            }
            if (child2_pid != -1) {
                kill(child2_pid, SIGUSR1);
            }
            exit(EXIT_SUCCESS);
        }

        pid_t pid;
        int status;

        while ((pid = waitpid(-1, &status, 0)) > 0) {
            my_printf("Child with PID %ld has terminated with status %d.\n", (long) pid,
                      WEXITSTATUS(status));
        }

        exit(EXIT_SUCCESS);
    }
}
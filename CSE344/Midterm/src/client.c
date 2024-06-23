#include "client.h"

DIR *arch_dir = NULL;
char *arch_dirname;

pid_t server_pid;
pid_t child_server_pid;
ConnectionOption connect_opt;
volatile sig_atomic_t is_connected = FALSE;

/* Semaphore and shared memory declarations for connection */
char sem_conn_request_name[SEM_CONN_REQUEST_NAME_LEN];
char sem_conn_response_name[SEM_CONN_RESPONSE_NAME_LEN];
char shm_conn_name[SHM_CONN_NAME_LEN];
int shm_conn_fd;
sem_t *sem_conn_req;
sem_t *sem_conn_res;
void *shm_conn_ptr;

/* Semaphore and shared memory declarations for communication */
char sem_comm_request_name[SEM_COMM_REQUEST_NAME_LEN];
char sem_comm_response_name[SEM_COMM_RESPONSE_NAME_LEN];
char shm_comm_name[SHM_COMM_NAME_LEN];
int shm_comm_fd;
sem_t *sem_comm_req;
sem_t *sem_comm_res;
void *shm_comm_ptr;

/* Semaphore and shared memory declarations for waiting client termination */
char shm_wait_term_name[SHM_WAIT_TERM_NAME_LEN];
char sem_wait_term_request_name[SEM_WAIT_TERM_REQUEST_NAME_LEN];
char sem_wait_term_response_name[SEM_WAIT_TERM_RESPONSE_NAME_LEN];
int shm_wait_term_fd;
sem_t *sem_wait_term_req;
sem_t *sem_wait_term_res;
void *shm_wait_term_ptr;

int main(int argc, char const *argv[]) {
    int while_counter = 0;
    pid_t pid = getpid();

    if (argc != 3) {
        my_printf("Invalid number of arguments. Usage: %s <connect/tryConnect> <serverPID>.\n",
                  argv[0]);
        exit(EXIT_FAILURE);
    }

    connect_opt = get_connection_action(argv[1]);

    if (connect_opt == UNKNOWN_CONNECTION) {
        my_printf(
            "Invalid argument. Connection option must be either 'connect' or 'tryConnect'.\n");
        exit(EXIT_FAILURE);
    }

    if (is_integer(argv[2]) == FALSE || parse_int(argv[2]) <= 0) {
        my_printf("Invalid argument. Server PID should be a positive number.\n");
        exit(EXIT_FAILURE);
    }

    server_pid = parse_int(argv[2]);

    if (atexit(cleanup) != 0) {
        display_error("Error registering cleanup function");
        return -1;
    }

    if (assign_signal_handler(SIGINT, sigint_handler) == FAILURE) {
        display_error("Error registering SIGINT handler");
        return -1;
    }

    if (assign_signal_handler(SIGTERM, sigterm_handler) == FAILURE) {
        display_error("Error registering SIGTERM handler");
        return -1;
    }

    if (assign_signal_handler(SIGUSR1, sigusr1_handler) == FAILURE) {
        display_error("Error registering SIGUSR1 handler");
        return -1;
    }

    if (assign_signal_handler(SIGUSR2, sigusr2_handler) == FAILURE) {
        display_error("Error registering SIGUSR2 handler");
        return -1;
    }

    my_snprintf(shm_conn_name, SHM_CONN_NAME_LEN, SHM_CONN_NAME_FORMAT, (long) server_pid);
    my_snprintf(sem_conn_request_name, SEM_CONN_REQUEST_NAME_LEN, SEM_CONN_REQUEST_NAME_FORMAT,
                (long) server_pid);
    my_snprintf(sem_conn_response_name, SEM_CONN_RESPONSE_NAME_LEN, SEM_CONN_RESPONSE_NAME_FORMAT,
                (long) server_pid);

    my_snprintf(shm_comm_name, SHM_COMM_NAME_LEN, SHM_COMM_NAME_FORMAT, (long) getpid());
    my_snprintf(sem_comm_request_name, SEM_COMM_REQUEST_NAME_LEN, SEM_COMM_REQUEST_NAME_FORMAT,
                (long) getpid());
    my_snprintf(sem_comm_response_name, SEM_COMM_RESPONSE_NAME_LEN, SEM_COMM_RESPONSE_NAME_FORMAT,
                (long) getpid());

    my_snprintf(shm_wait_term_name, SHM_WAIT_TERM_NAME_LEN, SHM_WAIT_TERM_NAME_FORMAT,
                (long) server_pid);
    my_snprintf(sem_wait_term_request_name, SEM_WAIT_TERM_REQUEST_NAME_LEN,
                SEM_WAIT_TERM_REQUEST_NAME_FORMAT, (long) server_pid);
    my_snprintf(sem_wait_term_response_name, SEM_WAIT_TERM_RESPONSE_NAME_LEN,
                SEM_WAIT_TERM_RESPONSE_NAME_FORMAT, (long) server_pid);

    shm_conn_fd = shm_open(shm_conn_name, O_RDWR, 0);
    shm_comm_fd = shm_open(shm_comm_name, O_RDWR | O_CREAT, SHM_PERMS);
    shm_wait_term_fd = shm_open(shm_wait_term_name, O_RDWR, 0);

    if (shm_conn_fd == -1) {
        my_printf("No server is running with the given PID. Exiting...\n");
        error_exit(NULL);
    }

    if (shm_comm_fd == -1) {
        error_exit("Error creating the shared memory for communication");
    }

    if (shm_wait_term_fd == -1) {
        error_exit("Error creating the shared memory for waiting client termination");
    }

    if (ftruncate(shm_comm_fd, SHM_COMM_SIZE) == -1) {
        error_exit("Error resizing the shared memory for the communication");
    }

    shm_conn_ptr = mmap(NULL, SHM_CONN_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_conn_fd, 0);
    shm_comm_ptr = mmap(NULL, SHM_COMM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_comm_fd, 0);
    shm_wait_term_ptr =
        mmap(NULL, SHM_WAIT_TERM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_wait_term_fd, 0);

    if (shm_conn_ptr == MAP_FAILED) {
        error_exit("Error mapping the shared memory for connection");
    }

    if (shm_comm_ptr == MAP_FAILED) {
        error_exit("Error mapping the shared memory for communication");
    }

    if (shm_wait_term_ptr == MAP_FAILED) {
        error_exit("Error mapping the shared memory for the waiting client termination");
    }

    if (close(shm_conn_fd) == -1) {
        error_exit("Error closing the shared memory file descriptor for connection");
    }

    if (close(shm_comm_fd) == -1) {
        error_exit("Error closing the shared memory file descriptor for communication");
    }

    if (close(shm_wait_term_fd) == -1) {
        error_exit(
            "Error closing the shared memory file descriptor for waiting client termination");
    }

    sem_conn_req = sem_open(sem_conn_request_name, 0);
    sem_conn_res = sem_open(sem_conn_response_name, 0);
    sem_comm_req = sem_open(sem_comm_request_name, O_CREAT, SEM_PERMS, 0);
    sem_comm_res = sem_open(sem_comm_response_name, O_CREAT, SEM_PERMS, 0);
    sem_wait_term_req = sem_open(sem_wait_term_request_name, 0);
    sem_wait_term_res = sem_open(sem_wait_term_response_name, 0);

    if (sem_conn_req == SEM_FAILED || sem_conn_res == SEM_FAILED) {
        error_exit("Error creating the semaphores for connection");
    }

    if (sem_comm_req == SEM_FAILED || sem_comm_res == SEM_FAILED) {
        error_exit("Error creating the semaphores for communication");
    }

    if (sem_wait_term_req == SEM_FAILED || sem_wait_term_res == SEM_FAILED) {
        error_exit("Error creating the semaphores for waiting client termination");
    }

    ConnectionRequest conn_req;
    memset(&conn_req, 0, sizeof(ConnectionRequest));
    conn_req.pid = pid;
    conn_req.option = connect_opt;

    sem_wait(sem_conn_res);
    memcpy(shm_conn_ptr, &conn_req, sizeof(ConnectionRequest));
    sem_post(sem_conn_req);

    while (TRUE) {
        if (sem_wait(sem_comm_res) == -1 && errno == EINTR) {
            continue;
        }

        is_connected = TRUE;
        CommunicationData init_response = *(CommunicationData *) shm_comm_ptr;
        child_server_pid = init_response.response.pid;
        break;
    }

    my_printf(">> Connected to server with PID %ld.\n", (long) getpid());

    while (TRUE) {
        Bool is_invalid = FALSE;
        char args[MAX_ARGS][NAME_MAX + 1];
        char input[MAX_INPUT_LEN];
        int args_count = 0;

        my_printf("%s>> Enter command: ", while_counter++ > 0 ? "\n" : "");
        my_fgets(STDIN_FILENO, input, MAX_INPUT_LEN);

        if (my_strlen(input) == 0) {
            continue;
        }

        char *head = my_strtok(input, '\"');
        char *rest = my_strtok(NULL, '\"'); // extra string argument for write command
        char *token = my_strtok(head, ' ');
        Command command = get_command(trim(token));

        if (command == UNKNOWN_COMMAND) {
            my_printf("\nInvalid command. Type 'help' to see the list of commands.\n");
            continue;
        }

        int arg_num = command_args[command];

        while ((token = my_strtok(NULL, ' ')) != NULL && args_count < arg_num) {
            my_snprintf(args[args_count++], NAME_MAX + 1, "%s", trim(token));
        }

        if (rest != NULL) {
            my_snprintf(args[args_count++], NAME_MAX + 1, "%s", rest);

            if (command != WRITE) {
                is_invalid = TRUE;
            }
        }

        if (command == WRITE && rest == NULL) {
            is_invalid = TRUE;
        }

        if (((args_count != arg_num) &&
             !(command == HELP && args_count == 0) &&   // exclude help without command
             !(command == READ && args_count == 1) &&   // exclude read without line number
             !(command == WRITE && args_count == 2)) || // exclude write without line number
            (token != NULL && args_count == arg_num) ||
            is_invalid) {
            my_printf("\nInvalid number of arguments for the command. Here are the possible "
                      "options for the command: \n");
            print_command_help(command);
            continue;
        }

        dispatch_command(command, args, args_count);
    }

    return 0;
}

Command get_command(const char *command) {
    for (int i = 0; i < COMMAND_NUM; i++) {
        if (my_strcmp(command, command_names[i]) == 0) {
            return i;
        }
    }

    return UNKNOWN_COMMAND;
}

ConnectionOption get_connection_action(const char *action) {
    if (my_strcmp(action, "connect") == 0) {
        return CONNECT;
    } else if (my_strcmp(action, "tryConnect") == 0) {
        return TRY_CONNECT;
    }

    return UNKNOWN_CONNECTION;
}

FunctionStatus clean_archive_dir() {
    if (arch_dir == NULL) {
        return SUCCESS;
    }

    char filepath[PATH_MAX];
    struct dirent *entry;

    while ((entry = readdir(arch_dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        my_snprintf(filepath, PATH_MAX, "%s/%s", arch_dirname, entry->d_name);

        if (unlink(filepath) == -1) {
            display_error("Error removing the archive file");
            return FAILURE;
        }
    }

    if (closedir(arch_dir) == -1) {
        display_error("Error closing the archive directory");
        return FAILURE;
    }

    arch_dir = NULL;

    if (rmdir(arch_dirname) == -1) {
        display_error("Error removing the archive directory");
        return FAILURE;
    }

    return SUCCESS;
}

void archive(char filename[NAME_MAX + 1]) {
    int fd;
    int file_count = 0;
    int status;
    int total_bytes = 0;
    char filepath[PATH_MAX];
    char last_filename[NAME_MAX + 1];
    char temp_filename[NAME_MAX + 1];
    char *extension;
    pid_t child_pid;
    CommunicationData data;

    memset(last_filename, 0, NAME_MAX + 1);
    memset(temp_filename, 0, NAME_MAX + 1);
    memset(&data, 0, sizeof(CommunicationData));
    data.request.command = ARCHIVE;
    data.request.args_num = 0;

    memcpy(temp_filename, filename, my_strlen(filename));
    memcpy(shm_comm_ptr, &data, sizeof(CommunicationData));

    arch_dirname = my_strtok(temp_filename, '.');
    extension = my_strtok(NULL, '.');

    if (extension == NULL || my_strcmp(extension, "tar") != 0) {
        my_printf("\nInvalid filename. Filename should be in the format <filename>.tar\n");
        return;
    }

    if (mkdir(arch_dirname, DIR_PERMS) == -1) {
        error_exit("\nError creating the archive directory");
    }

    arch_dir = opendir(arch_dirname);

    if (!arch_dir) {
        error_exit("Error opening the archive directory");
    }

    my_printf("\nArchiving the current contents of the server...\n");
    my_printf("Creating archive directory...\n");

    do {
        sem_post(sem_comm_req);
        sem_wait(sem_comm_res);
        memset(filepath, 0, PATH_MAX);
        data = *(CommunicationData *) shm_comm_ptr;
        total_bytes += data.bytes;

        if (data.response.status != RESPONSE_CONTINUE) {
            break;
        }

        if (my_strcmp(last_filename, data.response.filename) != 0) {
            file_count++;
            memcpy(last_filename, data.response.filename, NAME_MAX + 1);
        }

        my_snprintf(filepath, PATH_MAX, "%s/%s", arch_dirname, data.response.filename);
        fd = open(filepath, WRITE_ONLY, FILE_PERMS);

        if (fd == -1) {
            error_exit("Error opening the file for writing");
        }

        if (write(fd, data.buffer, data.bytes) == -1) {
            error_exit("Error writing to the file");
        }

        if (close(fd) == -1) {
            error_exit("Error closing the file descriptor");
        }
    } while (data.response.status == RESPONSE_CONTINUE);

    if (data.response.status == RESPONSE_NOT_FOUND) {
        my_printf("%s", data.buffer);
        return;
    }

    my_printf("%d files downloaded... %d bytes transferred...\n", file_count, total_bytes);
    my_printf("Calling tar utility...");

    switch ((child_pid = fork())) {
    case -1:
        error_exit("Error forking the process");
        break;
    case 0:
        execlp("tar", "tar", "-cf", filename, arch_dirname, NULL);
        error_exit("Error executing the tar utility");
        exit(EXIT_FAILURE);
        break;
    default:
        my_printf("Child PID %ld\n", (long) child_pid);
        break;
    }

    waitpid(child_pid, &status, 0);

    if (status != 0) {
        my_printf("Child returned with FAILURE...\n");
        return;
    }

    my_printf("Child returned with SUCCESS...\n");
    my_printf("Copying the archive file...\n");
    my_printf("Removing the archive directory...\n");

    if (clean_archive_dir() == FAILURE) {
        error_exit("Error cleaning the archive directory");
    }

    my_printf("SUCCESS Server side files are archived in \"%s\"\n", filename);
}

void cleanup() {
    clean_archive_dir();

    sem_close(sem_conn_req);
    sem_close(sem_conn_res);
    sem_close(sem_comm_req);
    sem_close(sem_comm_res);
    sem_close(sem_wait_term_req);
    sem_close(sem_wait_term_res);

    sem_unlink(sem_comm_request_name);
    sem_unlink(sem_comm_response_name);

    close(shm_conn_fd);
    close(shm_comm_fd);
    close(shm_wait_term_fd);

    munmap(shm_conn_ptr, SHM_CONN_SIZE);
    munmap(shm_comm_ptr, SHM_COMM_SIZE);
    munmap(shm_wait_term_ptr, SHM_WAIT_TERM_SIZE);

    shm_unlink(shm_comm_name);
}

void dispatch_command(Command command, char args[MAX_ARGS][NAME_MAX + 1], int args_num) {
    switch (command) {
    case HELP:
        args_num == 0 ? print_all_commands() : print_command_help(get_command(args[0]));
        break;
    case LIST:
        list();
        break;
    case READ:
        readF(args, args_num);
        break;
    case WRITE:
        writeT(args, args_num);
        break;
    case UPLOAD:
        upload(args[0]);
        break;
    case DOWNLOAD:
        download(args[0]);
        break;
    case ARCHIVE:
        archive(args[0]);
        break;
    case KILL_SERVER:
        kill_server();
        break;
    case QUIT:
        quit();
        break;
    default:
        break;
    }
}

void download(char filename[NAME_MAX + 1]) {
    int fd;
    int total_bytes = 0;
    int while_counter = 0;
    CommunicationData data;

    memset(&data, 0, sizeof(CommunicationData));
    data.request.command = DOWNLOAD;
    data.request.args_num = 1;

    memcpy(data.request.args[0], filename, my_strlen(filename));
    memcpy(shm_comm_ptr, &data, sizeof(CommunicationData));

    do {
        sem_post(sem_comm_req);
        sem_wait(sem_comm_res);
        data = *(CommunicationData *) shm_comm_ptr;

        if (data.response.status == RESPONSE_NOT_FOUND) {
            break;
        }

        if (while_counter == 0) {
            my_printf("\nFile download request received. Beginning file download...\n");
            fd = open(filename, OVERWRITE, FILE_PERMS);

            if (fd == -1) {
                error_exit("Error opening the file for writing");
            }
        }

        total_bytes += data.bytes;

        if (write(fd, data.buffer, data.bytes) == -1) {
            error_exit("Error writing to the file");
        }

        while_counter++;
    } while (data.response.status == RESPONSE_CONTINUE);

    if (data.response.status != RESPONSE_NOT_FOUND) {
        my_printf("%d bytes downloaded\n", total_bytes);

        if (close(fd) == -1) {
            error_exit("Error closing the file descriptor");
        }
    } else {
        my_printf("\nFile not found\n");
    }
}

void error_exit(const char *format, ...) {
    va_list args;
    va_start(args, format);

    if (format != NULL) {
        vdisplay_error(format, args);
    }

    if (is_connected == TRUE) {
        kill(child_server_pid, SIGUSR2);
    }

    va_end(args);
    exit(EXIT_FAILURE);
}

void kill_server() {
    CommunicationData data;
    memset(&data, 0, sizeof(CommunicationData));
    data.request.command = KILL_SERVER;

    memcpy(shm_comm_ptr, &data, sizeof(CommunicationData));
    sem_post(sem_comm_req);
    sem_wait(sem_comm_res);
}

void list() {
    CommunicationData data;
    memset(&data, 0, sizeof(CommunicationData));
    data.request.command = LIST;

    memcpy(shm_comm_ptr, &data, sizeof(CommunicationData));
    my_printf("\n");

    do {
        sem_post(sem_comm_req);
        sem_wait(sem_comm_res);
        data = *(CommunicationData *) shm_comm_ptr;
        my_printf("%s", data.buffer);
    } while (data.response.status == RESPONSE_CONTINUE);
}

void print_all_commands() {
    my_printf("\nAvailable commands are:\n");
    for (int i = 0; i < COMMAND_NUM; i++) {
        my_printf("%s%s", command_names[i], i == COMMAND_NUM - 1 ? "\n" : ", ");
    }
}

void print_command_help(Command command) {
    switch (command) {
    case HELP:
        my_printf(
            "\nhelp <command>\n  Displays the list of available commands or the help message of "
            "<command> if <command> is given.\n");
        break;
    case LIST:
        my_printf("\nlist\n  Lists the files in the server's directory.\n");
        break;
    case READ:
        my_printf("\nreadF <file> <line #>\n  Displays the #th line of the <file>, if no line "
                  "number is given the whole contents of the <file> is displayed. Returns with an "
                  "error if <file> does not exist.\n");
        break;
    case WRITE:
        my_printf("\nwriteT <file> <line #> <string>\n  Writes the content of <string> to the #th "
                  "line of the <file>, if no line number is given writes to the end of <file>. If "
                  "the <file> does not exists in the server's directory creates and edits the "
                  "<file> at the same time. <string> must be enclosed in double quotes.\n");
        break;
    case UPLOAD:
        my_printf("\nupload <file>\n  Uploads the <file> from the current working directory of the "
                  "client to the server's directory.\n");
        break;
    case DOWNLOAD:
        my_printf("\ndownload <file>\n  Downloads the <file> from the server directory to the "
                  "current working directory of the client.\n");
        break;
    case ARCHIVE:
        my_printf("\narchServer <filename>\n  Creates a tar archive named <filename>.tar "
                  "containing all the files in the server's directory.\n");
        break;
    case KILL_SERVER:
        my_printf("\nkillServer\n  Kills the server.\n");
        break;
    case QUIT:
        my_printf("\nquit\n  Disconnects from the server and exits the client.\n");
        break;
    case UNKNOWN_COMMAND:
        my_printf("\nInvalid command. Type 'help' to see the list of commands.\n");
        break;
    }
}

void quit() {
    CommunicationData data;
    memset(&data, 0, sizeof(CommunicationData));
    data.request.command = QUIT;

    my_printf("\nSending write request to server log file\n");
    my_printf("Waiting for logfile ...\n");
    memcpy(shm_comm_ptr, &data, sizeof(CommunicationData));
    sem_post(sem_comm_req);
    sem_wait(sem_comm_res);
    my_printf("Logfile write request granted\n");
    my_printf("Bye\n");
    exit(EXIT_SUCCESS);
}

void readF(char args[MAX_ARGS][NAME_MAX + 1], int args_num) {
    if (args_num == 2 && (is_integer(args[1]) == FALSE || parse_int(args[1]) <= 0)) {
        my_printf("\nInvalid argument. Line number should be a positive number.\n");
        return;
    }

    int total_bytes = 0;
    CommunicationData data;

    memset(&data, 0, sizeof(CommunicationData));
    data.request.command = READ;
    data.request.args_num = args_num;

    for (int i = 0; i < args_num; i++) {
        memcpy(data.request.args[i], args[i], my_strlen(args[i]));
    }

    memcpy(shm_comm_ptr, &data, sizeof(CommunicationData));
    my_printf("\n");

    do {
        sem_post(sem_comm_req);
        sem_wait(sem_comm_res);
        data = *(CommunicationData *) shm_comm_ptr;
        total_bytes += data.bytes;
        my_printf("%s", data.buffer);
    } while (data.response.status == RESPONSE_CONTINUE);

    if (data.response.status != RESPONSE_NOT_FOUND) {
        my_printf("\n\nBytes read: %d\n", total_bytes);
    }
}

void sigint_handler(int sig) {
    if (is_connected == TRUE) {
        my_printf("\nDisconnecting from server...\n");
        kill(child_server_pid, SIGTERM);
    } else {
        pid_t pid = getpid();
        sem_wait(sem_wait_term_res);
        memcpy(shm_wait_term_ptr, &pid, sizeof(pid_t));
        sem_post(sem_wait_term_req);
        kill(server_pid, SIGUSR1);
    }

    exit(EXIT_SUCCESS);
}

void sigterm_handler(int sig) {
    my_printf("\nServer terminated. Exiting...\n");
    my_printf("Bye\n");
    exit(EXIT_SUCCESS);
}

void sigusr1_handler(int sig) {
    if (connect_opt == CONNECT) {
        my_printf("Waiting for queue...\n");
    } else if (connect_opt == TRY_CONNECT) {
        my_printf("Server is full. Exiting...\n");
        exit(EXIT_SUCCESS);
    }
}

void sigusr2_handler(int sig) {
    my_printf("Server encountered an error. Exiting...\n");
    my_printf("Bye\n");
    exit(EXIT_SUCCESS);
}

void upload(char filename[NAME_MAX + 1]) {
    if (file_exists(filename) == FALSE) {
        my_printf("\nFile not found\n");
        return;
    }

    int bytes_read;
    int fd;
    int total_bytes = 0;
    CommunicationData data;

    fd = open(filename, READ_ONLY, FILE_PERMS);

    if (fd == -1) {
        error_exit("Error opening the file for reading");
    }

    my_printf("\nFile transfer request received. Beginning file transfer...\n");
    flock(fd, LOCK_EX);

    do {
        memset(&data, 0, sizeof(CommunicationData));
        data.request.command = UPLOAD;
        data.request.args_num = 1;

        bytes_read = read(fd, data.buffer, MAX_OUTPUT_LEN);

        if (bytes_read == -1) {
            error_exit("Error reading from the file %s", filename);
        }

        total_bytes += bytes_read;
        data.bytes = bytes_read;

        memcpy(data.request.args[0], filename, my_strlen(filename));
        memcpy(shm_comm_ptr, &data, sizeof(CommunicationData));
        sem_post(sem_comm_req);

        if (bytes_read != 0) {
            sem_wait(sem_comm_res);
        }
    } while (bytes_read != 0);

    flock(fd, LOCK_UN);
    my_printf("%d bytes transferred\n", total_bytes);
}

void writeT(char args[MAX_ARGS][NAME_MAX + 1], int args_num) {
    if (args_num == 3 && (is_integer(args[1]) == FALSE || parse_int(args[1]) <= 0)) {
        my_printf("\nInvalid argument. Line number should be a positive number.\n");
        return;
    }

    CommunicationData data;
    memset(&data, 0, sizeof(CommunicationData));
    data.request.command = WRITE;
    data.request.args_num = args_num;

    for (int i = 0; i < args_num; i++) {
        memcpy(data.request.args[i], args[i], my_strlen(args[i]));
    }

    my_printf("\nFile write request received. Beginning file write...\n");

    memcpy(shm_comm_ptr, &data, sizeof(CommunicationData));
    sem_post(sem_comm_req);
    sem_wait(sem_comm_res);
    data = *(CommunicationData *) shm_comm_ptr;
    my_printf("%d bytes written\n", data.bytes);
}
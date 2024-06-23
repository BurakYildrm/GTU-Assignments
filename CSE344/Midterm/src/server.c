#include "server.h"

const char *dirname;
char dirlock_name[DIR_LOCK_NAME_LEN];
int max_clients;
int dirlock_fd = -1;
int fd;
int temp_fd;
DIR *dir;

int process_id;
pid_t client_pid;
pid_t parent_pid;
ProcessTable process_table;
volatile sig_atomic_t ready_count = 0;

/* Semaphore and shared memory declarations for connection */
char shm_conn_name[SHM_CONN_NAME_LEN];
char sem_conn_request_name[SEM_CONN_REQUEST_NAME_LEN];
char sem_conn_response_name[SEM_CONN_RESPONSE_NAME_LEN];
int shm_conn_fd;
sem_t *sem_conn_req;
sem_t *sem_conn_res;
void *shm_conn_ptr;

/* Semaphore and shared memory declarations for communication */
char shm_comm_name[SHM_COMM_NAME_LEN];
char sem_comm_request_name[SEM_COMM_REQUEST_NAME_LEN];
char sem_comm_response_name[SEM_COMM_RESPONSE_NAME_LEN];
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
    parent_pid = getpid();

    if (argc != 3) {
        my_printf("Invalid number of arguments. Usage: %s <dirname> <max. #ofClients>.\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (is_integer(argv[2]) == FALSE || parse_int(argv[2]) <= 0) {
        my_printf("Invalid argument. Maximum number of clients should be a positive number.\n");
        exit(EXIT_FAILURE);
    }

    dirname = argv[1];
    max_clients = parse_int(argv[2]);
    dir = opendir(dirname);

    if (atexit(cleanup) != 0) {
        display_error("Error registering the cleanup function");
        exit(EXIT_FAILURE);
    }

    if (!dir) {
        if (errno == ENOENT) {
            if (mkdir(dirname, DIR_PERMS) == -1) {
                display_error("Error creating the directory %s", dirname);
                exit(EXIT_FAILURE);
            }

            dir = opendir(dirname);

            if (!dir) {
                display_error("Error opening the directory %s after creation", dirname);
                exit(EXIT_FAILURE);
            }
        } else {
            display_error("Error opening the directory %s", dirname);
            exit(EXIT_FAILURE);
        }
    }

    if (assign_signal_handler(SIGCHLD, sigchld_handler) == FAILURE) {
        display_error("Error registering SIGCHLD handler for server");
        return -1;
    }

    if (assign_signal_handler(SIGINT, sigint_handler) == FAILURE) {
        display_error("Error registering SIGINT handler for server");
        return -1;
    }

    if (assign_signal_handler(SIGUSR1, sigusr1_handler) == FAILURE) {
        display_error("Error registering SIGUSR1 handler for server");
        return -1;
    }

    process_table.size = max_clients * 2;
    process_table.num_running = 0;
    process_table.num_waiting = 0;
    process_table.process_count = 0;
    process_table.entries = (ProcessTableEntry *) calloc(max_clients * 2, sizeof(ProcessTableEntry));

    if (process_table.entries == NULL) {
        display_error("Error allocating memory for the process table");
        exit(EXIT_FAILURE);
    }

    my_snprintf(dirlock_name, DIR_LOCK_NAME_LEN, DIR_LOCK_NAME_FORMAT, dirname);

    my_snprintf(shm_conn_name, SHM_CONN_NAME_LEN, SHM_CONN_NAME_FORMAT, (long) getpid());
    my_snprintf(sem_conn_request_name, SEM_CONN_REQUEST_NAME_LEN, SEM_CONN_REQUEST_NAME_FORMAT, (long) getpid());
    my_snprintf(sem_conn_response_name, SEM_CONN_RESPONSE_NAME_LEN, SEM_CONN_RESPONSE_NAME_FORMAT, (long) getpid());

    my_snprintf(shm_wait_term_name, SHM_WAIT_TERM_NAME_LEN, SHM_WAIT_TERM_NAME_FORMAT, (long) getpid());
    my_snprintf(sem_wait_term_request_name, SEM_WAIT_TERM_REQUEST_NAME_LEN, SEM_WAIT_TERM_REQUEST_NAME_FORMAT,
                (long) getpid());
    my_snprintf(sem_wait_term_response_name, SEM_WAIT_TERM_RESPONSE_NAME_LEN, SEM_WAIT_TERM_RESPONSE_NAME_FORMAT,
                (long) getpid());

    shm_conn_fd = shm_open(shm_conn_name, O_RDWR | O_CREAT, SHM_PERMS);
    shm_wait_term_fd = shm_open(shm_wait_term_name, O_RDWR | O_CREAT, SHM_PERMS);

    if (shm_conn_fd == -1) {
        display_error("Error creating the shared memory for connection");
        exit(EXIT_FAILURE);
    }

    if (shm_wait_term_fd == -1) {
        display_error("Error creating the shared memory for waiting client termination");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shm_conn_fd, SHM_CONN_SIZE) == -1) {
        display_error("Error resizing the shared memory for connection");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shm_wait_term_fd, SHM_WAIT_TERM_SIZE) == -1) {
        display_error("Error resizing the shared memory for waiting client termination");
        exit(EXIT_FAILURE);
    }

    shm_conn_ptr = mmap(NULL, SHM_CONN_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_conn_fd, 0);
    shm_wait_term_ptr = mmap(NULL, SHM_WAIT_TERM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_wait_term_fd, 0);

    if (shm_conn_ptr == MAP_FAILED) {
        display_error("Error mapping the shared memory for connection");
        exit(EXIT_FAILURE);
    }

    if (shm_wait_term_ptr == MAP_FAILED) {
        display_error("Error mapping the shared memory for the waiting client termination");
        exit(EXIT_FAILURE);
    }

    if (close(shm_conn_fd) == -1) {
        display_error("Error closing the shared memory file descriptor for connection");
        exit(EXIT_FAILURE);
    }

    if (close(shm_wait_term_fd) == -1) {
        display_error("Error closing the shared memory file descriptor for waiting client termination");
        exit(EXIT_FAILURE);
    }

    sem_conn_req = sem_open(sem_conn_request_name, O_CREAT, SEM_PERMS, 0);
    sem_conn_res = sem_open(sem_conn_response_name, O_CREAT, SEM_PERMS, 1);
    sem_wait_term_req = sem_open(sem_wait_term_request_name, O_CREAT, SEM_PERMS, 0);
    sem_wait_term_res = sem_open(sem_wait_term_response_name, O_CREAT, SEM_PERMS, 1);

    if (sem_conn_req == SEM_FAILED || sem_conn_res == SEM_FAILED) {
        display_error("Error creating the semaphores for connection");
        exit(EXIT_FAILURE);
    }

    if (sem_wait_term_req == SEM_FAILED || sem_wait_term_res == SEM_FAILED) {
        display_error("Error creating the semaphores for waiting client termination");
        exit(EXIT_FAILURE);
    }

    if (init_log() == FAILURE) {
        exit(EXIT_FAILURE);
    }

    if (log_message(INFO, "Server (PID: %ld) started.", (long) getpid()) == FAILURE) {
        exit(EXIT_FAILURE);
    }

    my_printf("Server Started PID %ld...\n", (long) getpid());
    my_printf("Waiting for clients...\n");

    pid_t child_pid;
    ConnectionRequest conn_req;

    while (TRUE) {
        if (ready_count > 0) {
            client_pid = process_table.entries[max_clients - ready_count].client_pid;
            process_id = process_table.entries[max_clients - ready_count].id;
        } else {
            if (sem_wait(sem_conn_req) == -1 && errno == EINTR) {
                continue;
            }

            conn_req = *((ConnectionRequest *) shm_conn_ptr);
            client_pid = conn_req.pid;
            sem_post(sem_conn_res);
        }

        if (ready_count == 0 && process_table.num_running >= max_clients && conn_req.option == TRY_CONNECT) {
            kill(client_pid, SIGUSR1);
            continue;
        }

        if (ready_count == 0 && process_table.num_running >= max_clients && conn_req.option == CONNECT) {
            add_process(&process_table, -1, client_pid);
            continue;
        }

        if (ready_count == 0) {
            process_id = process_table.process_count + 1;
        }

        switch ((child_pid = fork())) {
            case -1:
                display_error("Error forking a child process");
                kill(getpid(), SIGINT);
            case 0:
                child_server();
                break;
            default:
                break;
        }

        if (ready_count > 0) {
            process_table.entries[max_clients - ready_count].server_pid = child_pid;
        } else {
            add_process(&process_table, child_pid, client_pid);
        }

        if (ready_count > 0) {
            my_printf("Client PID %ld connected as \"client_%.2d\"\n", (long) client_pid,
                      process_table.entries[max_clients - ready_count].id);
            log_message(INFO, "Client (PID: %ld) connected to Child Server (PID %ld) as client_%.2d.",
                        (long) client_pid, (long) child_pid, process_table.entries[max_clients - ready_count].id);
        } else {
            my_printf("Client PID %ld connected as \"client_%.2d\"\n", (long) client_pid, +process_table.process_count);
            log_message(INFO, "Client (PID: %ld) connected to Child Server (PID %ld) as client_%.2d.",
                        (long) client_pid, (long) child_pid, process_table.process_count);
        }

        if (ready_count > 0) {
            ready_count--;
        }
    }

    return 0;
}

FunctionStatus send_file_content(const char *filename, int line_num, Bool is_archive) {
    CommunicationData data;
    memset(&data, 0, sizeof(CommunicationData));

    int bytes_read = 0;
    int bytes_written = 0;
    int line_count = 0;
    char buffer[MAX_OUTPUT_LEN];
    char filepath[PATH_MAX];
    char no_file_message[] = "File not found\n";
    char no_line_message[] = "Line not found\n";

    memset(buffer, 0, sizeof(buffer));
    memset(filepath, 0, sizeof(filepath));
    my_snprintf(filepath, PATH_MAX, "%s%s%s", dirname, dirname[my_strlen(dirname) - 1] == '/' ? "" : "/", filename);

    if (file_exists(filepath) == FALSE) {
        data.response.status = RESPONSE_NOT_FOUND;
        memcpy(data.buffer, no_file_message, my_strlen(no_file_message));
        memcpy(shm_comm_ptr, &data, sizeof(CommunicationData));
        sem_post(sem_comm_res);
        return FAILURE;
    }

    fd = open(filepath, O_RDONLY);

    if (fd == -1) {
        error_exit("Error opening the file %s", filename);
    }

    flock(fd, LOCK_EX);

    if (line_num == 0) {
        do {
            bytes_read = read(fd, data.buffer, MAX_OUTPUT_LEN);

            if (bytes_read == -1) {
                display_error("Error reading the file %s", filename);
                return FAILURE;
            }

            if (bytes_read == 0) {
                data.response.status = is_archive == FALSE ? RESPONSE_OK : RESPONSE_CONTINUE;
                data.bytes = bytes_read;

                if (is_archive == TRUE) {
                    my_snprintf(data.response.filename, NAME_MAX + 1, "%s", filename);
                }

                memcpy(shm_comm_ptr, &data, sizeof(CommunicationData));
                sem_post(sem_comm_res);

                if (is_archive == TRUE) {
                    sem_wait(sem_comm_req);
                }
            } else {
                data.response.status = RESPONSE_CONTINUE;
                data.bytes = bytes_read;

                if (is_archive == TRUE) {
                    my_snprintf(data.response.filename, NAME_MAX + 1, "%s", filename);
                }

                memcpy(shm_comm_ptr, &data, sizeof(CommunicationData));
                sem_post(sem_comm_res);
                sem_wait(sem_comm_req);
                memset(&data, 0, sizeof(CommunicationData));
            }
        } while (bytes_read != 0);
    } else {
        Bool is_found = FALSE;

        while (TRUE) {
            bytes_read = read(fd, buffer, sizeof(buffer));

            if (bytes_read == -1) {
                error_exit("Error reading the file %s", filename);
            }

            if (bytes_read == 0) {
                break;
            }

            for (int i = 0; i < bytes_read; i++) {
                if (buffer[i] == '\n') {
                    line_count++;
                }

                if (line_count == line_num) {
                    if (bytes_written == MAX_OUTPUT_LEN) {
                        data.response.status = RESPONSE_CONTINUE;
                        data.bytes = bytes_written;
                        memcpy(shm_comm_ptr, &data, sizeof(CommunicationData));
                        sem_post(sem_comm_res);
                        sem_wait(sem_comm_req);
                        bytes_written = 0;
                        memset(&data, 0, sizeof(CommunicationData));
                    }

                    data.buffer[bytes_written] = '\n';
                    break;
                }

                if (line_count == line_num - 1) {
                    is_found = TRUE;

                    if (bytes_written == MAX_OUTPUT_LEN) {
                        data.response.status = RESPONSE_CONTINUE;
                        data.bytes = bytes_written;
                        memcpy(shm_comm_ptr, &data, sizeof(CommunicationData));
                        sem_post(sem_comm_res);
                        sem_wait(sem_comm_req);
                        bytes_written = 0;
                        memset(&data, 0, sizeof(CommunicationData));
                    }

                    if (buffer[i] != '\n') {
                        data.buffer[bytes_written++] = buffer[i];
                    }
                }
            }

            if (line_count == line_num) {
                break;
            }
        }

        if (is_found == FALSE) {
            data.response.status = RESPONSE_NOT_FOUND;
            memcpy(data.buffer, no_line_message, my_strlen(no_line_message));
        } else {
            data.response.status = RESPONSE_OK;
            data.bytes = bytes_written;
        }

        memcpy(shm_comm_ptr, &data, sizeof(CommunicationData));
        sem_post(sem_comm_res);
    }

    flock(fd, LOCK_UN);

    if (close(fd) == -1) {
        error_exit("Error closing the file %s", filename);
    }

    return SUCCESS;
}

FunctionStatus write_content_to_file(const char *filename, char *content, int line_num) {
    CommunicationData data;
    memset(&data, 0, sizeof(CommunicationData));

    int bytes_read = 0;
    int bytes_written = 0;
    int line_count = 0;

    char buffer[MAX_OUTPUT_LEN];
    char filepath[PATH_MAX];
    char temp_buffer[MAX_OUTPUT_LEN];
    char temp_filename[NAME_MAX + 1];

    memset(buffer, 0, sizeof(buffer));
    memset(filepath, 0, sizeof(filepath));
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memset(temp_filename, 0, sizeof(temp_filename));

    my_snprintf(filepath, PATH_MAX, "%s%s%s", dirname, dirname[my_strlen(dirname) - 1] == '/' ? "" : "/", filename);
    my_snprintf(temp_filename, NAME_MAX, TEMP_FILE_FORMAT, (long) getpid());

    unescape_newlines(content);

    int content_size = my_strlen(content);
    Bool does_exist = file_exists(filepath);
    Bool is_found = FALSE;

    if (line_num == 0 || does_exist == FALSE) {
        fd = open(filepath, WRITE_ONLY, FILE_PERMS);
    } else {
        fd = open(filepath, READ_ONLY, FILE_PERMS);
    }

    if (fd == -1) {
        error_exit("Error opening the file %s", filename);
    }

    flock(fd, LOCK_EX);

    if (line_num == 0 || does_exist == FALSE) {
        bytes_written = write(fd, content, content_size);

        if (bytes_written == -1) {
            error_exit("Error writing to the file %s", filename);
        }
    } else {
        temp_fd = open(temp_filename, WRITE_ONLY, FILE_PERMS);

        if (temp_fd == -1) {
            error_exit("Error opening the temporary file %s", temp_filename);
        }

        while (TRUE) {
            bytes_read = read(fd, buffer, sizeof(buffer));

            if (bytes_read == -1) {
                error_exit("Error reading the file %s", filename);
            }

            if (bytes_read == 0) {
                break;
            }

            for (int i = 0; i < bytes_read; i++) {
                if (buffer[i] == '\n') {
                    line_count++;
                }

                if (line_count == line_num && is_found == FALSE) {
                    for (int j = 0; j < content_size; j++) {
                        if (bytes_written == MAX_OUTPUT_LEN) {
                            if (write(temp_fd, temp_buffer, bytes_written) == -1) {
                                error_exit("Error writing to the temporary file %s", temp_filename);
                            }

                            bytes_written = 0;
                        }

                        temp_buffer[bytes_written++] = content[j];
                    }

                    if (bytes_written == MAX_OUTPUT_LEN) {
                        if (write(temp_fd, temp_buffer, bytes_written) == -1) {
                            error_exit("Error writing to the temporary file %s", temp_filename);
                        }

                        bytes_written = 0;
                    }

                    temp_buffer[bytes_written++] = '\n';
                    is_found = TRUE;
                } else {
                    if (bytes_written == MAX_OUTPUT_LEN) {
                        if (write(temp_fd, temp_buffer, bytes_written) == -1) {
                            error_exit("Error writing to the temporary file %s", temp_filename);
                        }

                        bytes_written = 0;
                    }

                    temp_buffer[bytes_written++] = buffer[i];
                }
            }
        }

        if (write(temp_fd, temp_buffer, bytes_written) == -1) {
            error_exit("Error writing to the temporary file %s", temp_filename);
        }

        if (is_found == FALSE) {
            if (write(temp_fd, content, my_strlen(content)) == -1) {
                error_exit("Error writing to the temporary file %s", temp_filename);
            }
        }

        if (close(temp_fd) == -1) {
            error_exit("Error closing the temporary file %s", temp_filename);
        }

        unlink(filepath);
        rename(temp_filename, filepath);
    }

    data.bytes = my_strlen(content);
    data.response.status = RESPONSE_OK;
    memcpy(shm_comm_ptr, &data, sizeof(CommunicationData));
    sem_post(sem_comm_res);

    flock(fd, LOCK_UN);

    if (close(fd) == -1) {
        error_exit("Error closing the file %s", filename);
    }

    return does_exist == FALSE ? FILE_NOT_FOUND : is_found == TRUE ? SUCCESS : LINE_NOT_FOUND;
}

int add_process(ProcessTable *table, pid_t server_pid, pid_t client_pid) {
    int id = ++(table->process_count);
    int index = table->num_running + table->num_waiting;

    if (index >= table->size) {
        table->size *= 2;
        table->entries = (ProcessTableEntry *) realloc(table->entries, table->size * sizeof(ProcessTableEntry));
    }

    if (table->entries == NULL) {
        display_error("Error reallocating memory for the process table");
        kill(getpid(), SIGINT);
    }

    table->entries[index].id = id;
    table->entries[index].server_pid = server_pid;
    table->entries[index].client_pid = client_pid;

    if (table->num_running < max_clients) {
        table->num_running++;
        table->entries[index].state = RUNNING;
    } else {
        table->num_waiting++;
        table->entries[index].state = WAITING;
        kill(client_pid, SIGUSR1);
        my_printf("Connection request PID %ld...  Queue FULL\n", client_pid);
    }

    return index;
}

int cmp_entry_by_client_pid(const void *entry, const void *client_pid) {
    return ((ProcessTableEntry *) entry)->client_pid - *((pid_t *) client_pid);
}

int cmp_entry_by_server_pid(const void *entry, const void *server_pid) {
    return ((ProcessTableEntry *) entry)->server_pid - *((pid_t *) server_pid);
}

void archive() {
    Bool is_empty = TRUE;
    CommunicationData data;

    char *entry_name;
    char empty_message[] = "No files in the directory\n";
    struct dirent *entry;

    memset(&data, 0, sizeof(CommunicationData));
    dirlock_fd = open(dirlock_name, READ_WRITE, FILE_PERMS);

    if (dirlock_fd == -1) {
        error_exit("Error opening the directory lock %s", dirlock_name);
    }

    flock(dirlock_fd, LOCK_EX);

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        is_empty = FALSE;
        entry_name = entry->d_name;
        send_file_content(entry_name, 0, TRUE);
    }

    rewinddir(dir);
    flock(dirlock_fd, LOCK_UN);

    if (close(dirlock_fd) == -1) {
        error_exit("Error closing the directory lock");
    }

    dirlock_fd = -1;

    if (is_empty == TRUE) {
        data.response.status = RESPONSE_NOT_FOUND;
        data.bytes = 0;
        memcpy(data.buffer, empty_message, my_strlen(empty_message));
        memcpy(shm_comm_ptr, &data, sizeof(CommunicationData));
        sem_post(sem_comm_res);
        log_message(INFO,
                    "Client (PID: %ld) tried to archive all the files in the directory %s "
                    "but it was empty.",
                    (long) client_pid, dirname);
    } else {
        data.response.status = RESPONSE_OK;
        data.bytes = 0;
        memcpy(shm_comm_ptr, &data, sizeof(CommunicationData));
        sem_post(sem_comm_res);
        log_message(INFO, "Client (PID: %ld) archived all the files in the directory %s.", (long) client_pid, dirname);
    }
}

void child_server() {
    if (assign_signal_handler(SIGINT, SIG_IGN) == FAILURE) {
        error_exit("Error registering SIGINT handler for child server");
    }

    if (assign_signal_handler(SIGTERM, sigterm_handler) == FAILURE) {
        error_exit("Error registering SIGTERM handler for child server");
    }

    if (assign_signal_handler(SIGUSR2, sigusr2_handler) == FAILURE) {
        error_exit("Error registering SIGUSR2 handler for child server");
    }

    my_snprintf(shm_comm_name, SHM_COMM_NAME_LEN, SHM_COMM_NAME_FORMAT, (long) client_pid);
    my_snprintf(sem_comm_request_name, SEM_COMM_REQUEST_NAME_LEN, SEM_COMM_REQUEST_NAME_FORMAT, (long) client_pid);
    my_snprintf(sem_comm_response_name, SEM_COMM_RESPONSE_NAME_LEN, SEM_COMM_RESPONSE_NAME_FORMAT, (long) client_pid);

    shm_comm_fd = shm_open(shm_comm_name, O_RDWR, 0);

    if (shm_comm_fd == -1) {
        error_exit("Error creating the shared memory for communication");
    }

    shm_comm_ptr = mmap(NULL, SHM_COMM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_comm_fd, 0);

    if (shm_comm_ptr == MAP_FAILED) {
        error_exit("Error mapping the shared memory for communication");
    }

    if (close(shm_comm_fd) == -1) {
        error_exit("Error closing the shared memory file descriptor for communication");
    }

    sem_comm_req = sem_open(sem_comm_request_name, 0);
    sem_comm_res = sem_open(sem_comm_response_name, 0);

    if (sem_comm_req == SEM_FAILED || sem_comm_res == SEM_FAILED) {
        error_exit("Error creating the semaphores for communication");
    }

    CommunicationData init_response;
    memset(&init_response, 0, sizeof(CommunicationData));
    init_response.response.pid = getpid();
    memcpy(shm_comm_ptr, &init_response, sizeof(CommunicationData));
    sem_post(sem_comm_res);

    while (TRUE) {
        CommunicationData *data;
        data = ((CommunicationData *) shm_comm_ptr);
        sem_wait(sem_comm_req);
        process_command(data->request.command, data->request.args, data->request.args_num);
    }
}

void cleanup() {
    pid_t pid = getpid();

    if (pid == parent_pid) {
        cleanup_parent();
    } else {
        cleanup_child();
    }
}

void cleanup_child() {
    sem_close(sem_comm_req);
    sem_close(sem_comm_res);

    close(shm_comm_fd);
    munmap(shm_comm_ptr, SHM_COMM_SIZE);

    if (dirlock_fd != -1) {
        rewinddir(dir);
        close(dirlock_fd);
    }

    close(fd);
    close(temp_fd);

    free(process_table.entries);
}

void cleanup_parent() {
    sem_close(sem_conn_req);
    sem_close(sem_conn_res);
    sem_close(sem_wait_term_req);
    sem_close(sem_wait_term_res);

    sem_unlink(sem_conn_request_name);
    sem_unlink(sem_conn_response_name);
    sem_unlink(sem_wait_term_request_name);
    sem_unlink(sem_wait_term_response_name);

    close(shm_conn_fd);
    close(shm_wait_term_fd);

    munmap(shm_conn_ptr, SHM_CONN_SIZE);
    munmap(shm_wait_term_ptr, SHM_WAIT_TERM_SIZE);

    shm_unlink(shm_conn_name);
    shm_unlink(shm_wait_term_name);

    free(process_table.entries);

    closedir(dir);
    unlink(dirlock_name);
}

void download(char args[MAX_ARGS][NAME_MAX + 1]) {
    char filename[NAME_MAX + 1];

    memset(filename, 0, sizeof(filename));
    memcpy(filename, args[0], my_strlen(args[0]));

    FunctionStatus status = send_file_content(filename, 0, FALSE);

    if (status == FAILURE) {
        log_message(INFO, "Client (PID: %ld) tried to download the file %s but it was not found.", (long) client_pid,
                    filename);
    } else {
        log_message(INFO, "Client (PID: %ld) downloaded the file %s.", (long) client_pid, filename);
    }
}

void error_exit(const char *format, ...) {
    va_list args;
    va_start(args, format);

    vdisplay_error(format, args);

    va_end(args);
    exit(EXIT_FAILURE);
}

void kill_server() {
    log_message(INFO, "Client (PID: %ld) requested to kill the server.", (long) client_pid);
    my_printf("Kill signal from client_%.2d", process_id);
    kill(parent_pid, SIGINT);
}

void list() {
    CommunicationData data;
    struct dirent *entry;
    char *entry_name;
    char empty_message[] = "No files in the directory\n";
    int bytes_written = 0;
    Bool is_empty = TRUE;

    memset(&data, 0, sizeof(CommunicationData));
    dirlock_fd = open(dirlock_name, READ_WRITE, FILE_PERMS);

    if (dirlock_fd == -1) {
        error_exit("Error opening the directory lock %s", dirlock_name);
    }

    flock(dirlock_fd, LOCK_EX);

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        is_empty = FALSE;
        entry_name = entry->d_name;

        for (int i = 0; i < my_strlen(entry_name); i++) {
            if (bytes_written == MAX_OUTPUT_LEN) {
                data.response.status = RESPONSE_CONTINUE;
                memcpy(shm_comm_ptr, &data, sizeof(CommunicationData));
                sem_post(sem_comm_res);
                sem_wait(sem_comm_req);
                bytes_written = 0;
                memset(&data, 0, sizeof(CommunicationData));
            }

            data.buffer[bytes_written++] = entry_name[i];
        }

        if (bytes_written == MAX_OUTPUT_LEN) {
            data.response.status = RESPONSE_CONTINUE;
            memcpy(shm_comm_ptr, &data, sizeof(CommunicationData));
            sem_post(sem_comm_res);
            sem_wait(sem_comm_req);
            bytes_written = 0;
            memset(&data, 0, sizeof(CommunicationData));
        }

        data.buffer[bytes_written++] = '\n';
    }

    rewinddir(dir);
    flock(dirlock_fd, LOCK_UN);

    if (close(dirlock_fd) == -1) {
        error_exit("Error closing the directory lock");
    }

    dirlock_fd = -1;

    if (is_empty == TRUE) {
        memcpy(data.buffer, empty_message, my_strlen(empty_message));
    }

    data.response.status = RESPONSE_OK;
    memcpy(shm_comm_ptr, &data, sizeof(CommunicationData));
    sem_post(sem_comm_res);
    log_message(INFO, "Client (PID: %ld) listed all the files in the directory %s.", (long) client_pid, dirname);
}

void process_command(Command command, char args[MAX_ARGS][NAME_MAX + 1], int args_num) {
    switch (command) {
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
            upload();
            break;
        case DOWNLOAD:
            download(args);
            break;
        case ARCHIVE:
            archive();
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

void quit() {
    CommunicationData data;
    memset(&data, 0, sizeof(CommunicationData));

    data.response.status = RESPONSE_OK;
    memcpy(shm_comm_ptr, &data, sizeof(CommunicationData));
    sem_post(sem_comm_res);
    exit(EXIT_SUCCESS);
}

void readF(char args[MAX_ARGS][NAME_MAX + 1], int args_num) {
    int line_num = args_num == 2 ? parse_int(args[1]) : 0;
    char filename[NAME_MAX + 1];

    memset(filename, 0, sizeof(filename));
    memcpy(filename, args[0], my_strlen(args[0]));

    FunctionStatus status = send_file_content(filename, line_num, FALSE);

    if (status == FAILURE) {
        log_message(INFO, "Client (PID: %ld) tried to read the file %s but it was not found.", (long) client_pid,
                    filename);
    } else if (status == SUCCESS && args_num == 1) {
        log_message(INFO, "Client (PID: %ld) read the file %s.", (long) client_pid, filename);
    } else if (status == SUCCESS && args_num == 2) {
        log_message(INFO, "Client (PID: %ld) read the line %d from the file %s.", (long) client_pid, line_num,
                    filename);
    }
}

void sigchld_handler(int sig) {
    int index;
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        index = indexof_struct(process_table.entries, process_table.num_running + process_table.num_waiting,
                               sizeof(ProcessTableEntry), &pid, cmp_entry_by_server_pid);

        ProcessTableEntry entry = process_table.entries[index];
        process_table.num_running--;

        for (int i = index; i <= process_table.num_running + process_table.num_waiting - 1; i++) {
            process_table.entries[i] = process_table.entries[i + 1];
        }

        if (status != 0) {
            kill(entry.client_pid, SIGUSR2);
            log_message(ERROR, "Child Server (PID: %ld) encountered an error.", (long) pid);
        }

        my_printf("client_%.2d disconnected...\n", entry.id);
        log_message(INFO, "Client (PID: %ld) disconnected.", (long) entry.client_pid);
    }

    for (int i = 0; i < max_clients; i++) {
        if (process_table.entries[i].state == WAITING) {
            process_table.entries[i].state = RUNNING;
            process_table.num_running++;
            process_table.num_waiting--;
            ready_count++;
        }
    }
}

void sigint_handler(int sig) {
    my_printf("\nTerminating...\n");

    for (int i = 0; i < process_table.num_running + process_table.num_waiting; i++) {
        ProcessTableEntry entry = process_table.entries[i];

        if (entry.state == RUNNING) {
            kill(entry.server_pid, SIGTERM);
        } else {
            kill(entry.client_pid, SIGTERM);
        }
    }

    log_message(INFO, "Server (PID: %ld) terminated.", (long) getpid());
    my_printf("Bye\n");
    exit(EXIT_SUCCESS);
}

void sigterm_handler(int sig) {
    kill(client_pid, SIGTERM);
    exit(EXIT_SUCCESS);
}

void sigusr1_handler(int sig) {
    int index;
    pid_t client_pid;

    sem_wait(sem_wait_term_req);
    client_pid = *((pid_t *) shm_wait_term_ptr);
    sem_post(sem_wait_term_res);

    index = indexof_struct(process_table.entries, process_table.num_running + process_table.num_waiting,
                           sizeof(ProcessTableEntry), &client_pid, cmp_entry_by_client_pid);

    for (int i = index; i < process_table.num_running + process_table.num_waiting - 1; i++) {
        process_table.entries[i] = process_table.entries[i + 1];
    }

    process_table.num_waiting--;
}

void sigusr2_handler(int sig) {
    log_message(ERROR, "Client (PID: %ld) encountered an error.", (long) client_pid);
    exit(EXIT_SUCCESS);
}

void unescape_newlines(char *str) {
    int len = my_strlen(str);
    int index = 0;

    for (int i = 0; i < len; i++) {
        if (i != len - 1 && str[i] == '\\' && str[i + 1] == 'n') {
            str[index++] = '\n';
            i++;
            continue;
        }
        str[index++] = str[i];
    }

    if (index < len) {
        str[index] = '\0';
    }
}

void upload() {
    int while_count = 0;
    char filepath[PATH_MAX];
    Bool does_exist;
    CommunicationData *data;

    memset(filepath, 0, sizeof(filepath));

    do {
        data = ((CommunicationData *) shm_comm_ptr);

        if (while_count == 0) {
            my_snprintf(filepath, PATH_MAX, "%s%s%s", dirname, dirname[my_strlen(dirname) - 1] == '/' ? "" : "/",
                        data->request.args[0]);

            if ((does_exist = file_exists(filepath)) == TRUE) {
                fd = open(filepath, OVERWRITE, FILE_PERMS);
            } else {
                fd = open(filepath, WRITE_ONLY, FILE_PERMS);
            }

            if (fd == -1) {
                error_exit("Error opening the file %s", filepath);
            }

            flock(fd, LOCK_EX);
        }

        if (write(fd, data->buffer, data->bytes) == -1) {
            error_exit("Error writing to the file %s", filepath);
        }

        sem_post(sem_comm_res);

        if (data->bytes != 0) {
            sem_wait(sem_comm_req);
        }

        while_count++;
    } while (data->bytes != 0);

    flock(fd, LOCK_UN);

    if (does_exist == FALSE) {
        log_message(INFO, "Client (PID: %ld) uploaded the file %s.", (long) client_pid, data->request.args[0]);
    } else {
        log_message(INFO, "Client (PID: %ld) updated the file %s.", (long) client_pid, data->request.args[0]);
    }
}

void writeT(char args[MAX_ARGS][NAME_MAX + 1], int args_num) {
    int line_num = args_num == 3 ? parse_int(args[1]) : 0;
    char content[NAME_MAX + 1];
    char filename[NAME_MAX + 1];
    char raw_content[NAME_MAX + 1];

    memset(content, 0, sizeof(content));
    memset(filename, 0, sizeof(filename));
    memset(raw_content, 0, sizeof(raw_content));

    memcpy(filename, args[0], my_strlen(args[0]));
    args_num == 3 ? memcpy(content, args[2], my_strlen(args[2])) : memcpy(content, args[1], my_strlen(args[1]));
    memcpy(raw_content, content, my_strlen(content));

    FunctionStatus status = write_content_to_file(filename, content, line_num);

    if (status == FILE_NOT_FOUND) {
        log_message(INFO, "Client (PID: %ld) created the file %s and wrote \"%s\".", (long) client_pid, filename,
                    raw_content);
    } else {
        if (line_num == 0 || status == LINE_NOT_FOUND) {
            log_message(INFO, "Client (PID: %ld) wrote \"%s\" to the end of the file %s.", (long) client_pid,
                        raw_content, filename);
        } else {
            log_message(INFO, "Client (PID: %ld) wrote \"%s\" to the line %d of the file %s.", (long) client_pid,
                        raw_content, line_num, filename);
        }
    }
}

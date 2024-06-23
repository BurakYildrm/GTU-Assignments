#include "1901042609_main.h"

volatile sig_atomic_t is_finished = FALSE;
DIR *src_dir;
DIR *dest_dir;
CopyInfo *buffer;

int buffer_size;
int curr_occupancy = 0;
int worker_num;
int available_workers;
int total_bytes = 0;
int regular_num = 0;
int fifo_num = 0;
int dir_num = 0;

char *src_dirname;
char *dest_dirname;

pthread_t *worker_threads;
pthread_t manager_thread;

pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t general_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t buffer_not_empty_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_not_full_cond = PTHREAD_COND_INITIALIZER;

struct timespec start, end;

int main(int argc, char const *argv[]) {
    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
        display_error("Error getting the start time");
        _exit(EXIT_FAILURE);
    }

    Bool is_valid = validate_args(argc, argv);
    long seconds, nanoseconds;
    double elapsed;

    if (is_valid == FALSE) {
        return 1;
    }

    struct sigaction sa_sigint;
    sa_sigint.sa_handler = sigint_handler;
    sigfillset(&sa_sigint.sa_mask);
    sa_sigint.sa_flags = 0;

    if (sigaction(SIGINT, &sa_sigint, NULL) == -1) {
        display_error("Error registering SIGINT handler");
        return 1;
    }

    if (atexit(cleanup) != 0) {
        display_error("Error registering cleanup function");
        return 1;
    }

    buffer_size = parse_int(argv[1]);
    worker_num = parse_int(argv[2]);
    available_workers = worker_num;
    src_dirname = argv[3];
    dest_dirname = argv[4];

    if (src_dirname[my_strlen(src_dirname) - 1] == '/') {
        src_dirname[my_strlen(src_dirname) - 1] = '\0';
    }

    if (dest_dirname[my_strlen(dest_dirname) - 1] == '/') {
        dest_dirname[my_strlen(dest_dirname) - 1] = '\0';
    }

    src_dir = opendir(src_dirname);
    dest_dir = opendir(dest_dirname);

    if (!src_dir) {
        if (errno == ENOENT) {
            my_printf("The source directory %s does not exist\n", src_dirname);
            exit(EXIT_FAILURE);
        } else {
            display_error("Failed to open source directory");
            exit(EXIT_FAILURE);
        }
    }

    if (!dest_dir) {
        if (errno == ENOENT) {
            if (mkdir(dest_dirname, DIR_PERMS) == -1) {
                display_error("Error creating the directory %s", dest_dirname);
                exit(EXIT_FAILURE);
            }

            dest_dir = opendir(dest_dirname);

            if (!dest_dir) {
                display_error("Error opening the directory %s after creation", dest_dirname);
                exit(EXIT_FAILURE);
            }
        } else {
            display_error("Error opening the directory %s", dest_dirname);
            exit(EXIT_FAILURE);
        }
    }

    buffer = (CopyInfo *) calloc(buffer_size, sizeof(CopyInfo));
    worker_threads = (pthread_t *) calloc(worker_num, sizeof(pthread_t));

    if (buffer == NULL || worker_threads == NULL) {
        display_error("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < buffer_size; i++) {
        buffer[i].src_fd = -1;
    }

    if (pthread_create(&manager_thread, NULL, manager, NULL) != 0) {
        display_error("Error creating the manager thread");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < worker_num; i++) {
        if (pthread_create(&worker_threads[i], NULL, worker, NULL) != 0) {
            display_error("Error creating worker thread %d", i);
            exit(EXIT_FAILURE);
        }
    }

    if (pthread_join(manager_thread, NULL) != 0) {
        display_error("Error joining manager thread");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < worker_num; i++) {
        if (pthread_join(worker_threads[i], NULL) != 0) {
            display_error("Error joining worker thread %d", i);
            exit(EXIT_FAILURE);
        }
    }

    if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
        perror("clock_gettime end");
        exit(EXIT_FAILURE);
    }

    seconds = end.tv_sec - start.tv_sec;
    nanoseconds = end.tv_nsec - start.tv_nsec;

    if (nanoseconds < 0) {
        seconds -= 1;
        nanoseconds += 1000000000;
    }

    elapsed = seconds * 1000.0 + nanoseconds / 1000000.0;
    long minutes = (long) (elapsed / 60000);
    long secs = (long) ((elapsed - minutes * 60000) / 1000);
    long millis = (long) (elapsed - minutes * 60000 - secs * 1000);

    my_printf("\n---------------STATISTICS--------------------\n");
    my_printf("Consumers: %d - Buffer Size: %d\n", worker_num, buffer_size);
    my_printf("Number of Regular File: %d\n", regular_num);
    my_printf("Number of FIFO File: %d\n", fifo_num);
    my_printf("Number of Directory: %d\n", dir_num);
    my_printf("TOTAL BYTES COPIED: %d\n", total_bytes);
    my_printf("TOTAL TIME: %02ld:%02ld.%03ld (min:sec.mili)\n", minutes, secs, millis);

    return 0;
}

Bool validate_args(int argc, char const *argv[]) {
    if (argc != 5) {
        my_printf("Usage: %s <bufferSize> <# of workers> <src> <dest>\n", argv[0]);
        return FALSE;
    }

    Bool is_buffer_size_valid = is_integer(argv[1]) == TRUE && parse_int(argv[1]) > 0;
    Bool is_worker_num_valid = is_integer(argv[2]) == TRUE && parse_int(argv[2]) > 0;

    if (is_buffer_size_valid == FALSE || is_worker_num_valid == FALSE) {
        my_printf("Invalid arguments. %s\n", is_buffer_size_valid == FALSE && is_worker_num_valid == FALSE
                                                 ? "Buffer size and worker number must be positive integers"
                                             : is_buffer_size_valid == FALSE
                                                 ? "Buffer size must be positive integer"
                                                 : "Worker number must be positive integer");
        return FALSE;
    }

    if (parse_int(argv[1]) < parse_int(argv[2])) {
        my_printf("Buffer size must be greater than or equal to the number of workers\n");
        return FALSE;
    }

    return TRUE;
}

void cleanup() {
    free(buffer);
    free(worker_threads);
    closedir(src_dir);
    closedir(dest_dir);
    pthread_mutex_destroy(&print_mutex);
    pthread_mutex_destroy(&general_mutex);
    pthread_cond_destroy(&buffer_not_empty_cond);
    pthread_cond_destroy(&buffer_not_full_cond);
}

void handle_directory(char *src, char *dest) {
    DIR *src_dir = opendir(src);
    DIR *dest_dir = opendir(dest);

    if (!src_dir) {
        if (errno == ENOENT) {
            my_printf("The source directory %s does not exist\n", src);
            return;
        } else {
            display_error("Error opening the source directory %s", src);
            return;
        }
    }

    if (!dest_dir) {
        if (errno == ENOENT) {
            if (mkdir(dest, DIR_PERMS) == -1) {
                display_error("Error creating the destination directory %s", dest);
                return;
            }

            dest_dir = opendir(dest);

            if (!dest_dir) {
                display_error("Error opening the destination directory %s after creation", dest);
                return;
            }
        } else {
            display_error("Error opening the destination directory %s", dest);
            return;
        }
    }

    struct dirent *entry;

    while ((entry = readdir(src_dir)) != NULL && is_finished != TRUE) {
        if (my_strcmp(entry->d_name, ".") == 0 || my_strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char src_path[PATH_MAX];
        char dest_path[PATH_MAX];

        memset(src_path, '\0', PATH_MAX);
        memset(dest_path, '\0', PATH_MAX);

        my_snprintf(src_path, PATH_MAX, "%s/%s", src, entry->d_name);
        my_snprintf(dest_path, PATH_MAX, "%s/%s", dest, entry->d_name);

        switch (get_file_type(src_path)) {
            case REGULAR:
                regular_num++;
                handle_regular(src_path, dest_path);
                break;
            case DIRECTORY:
                dir_num++;
                handle_directory(src_path, dest_path);
                break;
            case FIFO:
                fifo_num++;
                handle_fifo(src_path, dest_path);
                break;
            default:
                break;
        }
    }

    closedir(src_dir);
    closedir(dest_dir);
}

void handle_fifo(char *src, char *dest) {
    if (mkfifo(dest, FIFO_PERMS) == -1) {
        display_error("Error creating FIFO %s", dest);
        return;
    }

    int src_fd = open(src, O_RDONLY);
    int dest_fd = open(dest, O_WRONLY);

    if (src_fd == -1 || dest_fd == -1) {
        close(dest_fd);
        close(src_fd);

        if (src_fd == -1 && dest_fd == -1) {
            display_error("Error opening source and destination FIFOs %s and %s", src, dest);
        } else {
            display_error("Error opening the %s FIFO %s", src_fd == -1 ? "source" : "destination",
                          src_fd == -1 ? src : dest);
        }

        return;
    }

    CopyInfo copy_info;
    copy_info.src_fd = src_fd;
    copy_info.dest_fd = dest_fd;
    my_strcpy(copy_info.src, src);
    my_strcpy(copy_info.dest, dest);

    pthread_mutex_lock(&general_mutex);

    while (curr_occupancy == buffer_size || available_workers == 0) {
        pthread_cond_wait(&buffer_not_full_cond, &general_mutex);
    }

    buffer[curr_occupancy++] = copy_info;

    pthread_cond_signal(&buffer_not_empty_cond);
    pthread_mutex_unlock(&general_mutex);
}

void handle_regular(char *src, char *dest) {
    int src_fd = open(src, O_RDONLY);
    int dest_fd = open(dest, TRUNCATE, FILE_PERMS);

    if (src_fd == -1 || dest_fd == -1) {
        close(dest_fd);
        close(src_fd);

        if (src_fd == -1 && dest_fd == -1) {
            display_error("Error opening source and destination files %s and %s", src, dest);
        } else {
            display_error("Error opening the %s file %s", src_fd == -1 ? "source" : "destination",
                          src_fd == -1 ? src : dest);
        }

        return;
    }

    CopyInfo copy_info;
    copy_info.src_fd = src_fd;
    copy_info.dest_fd = dest_fd;
    my_strcpy(copy_info.src, src);
    my_strcpy(copy_info.dest, dest);

    pthread_mutex_lock(&general_mutex);

    while (curr_occupancy == buffer_size || available_workers == 0) {
        pthread_cond_wait(&buffer_not_full_cond, &general_mutex);
    }

    buffer[curr_occupancy++] = copy_info;

    pthread_cond_signal(&buffer_not_empty_cond);
    pthread_mutex_unlock(&general_mutex);
}

void *manager(void *arg) {
    struct dirent *entry;

    while ((entry = readdir(src_dir)) != NULL && is_finished != TRUE) {
        if (my_strcmp(entry->d_name, ".") == 0 || my_strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char src_path[PATH_MAX];
        char dest_path[PATH_MAX];

        memset(src_path, '\0', PATH_MAX);
        memset(dest_path, '\0', PATH_MAX);

        my_snprintf(src_path, PATH_MAX, "%s/%s", src_dirname, entry->d_name);
        my_snprintf(dest_path, PATH_MAX, "%s/%s", dest_dirname, entry->d_name);

        switch (get_file_type(src_path)) {
            case REGULAR:
                regular_num++;
                handle_regular(src_path, dest_path);
                break;
            case DIRECTORY:
                dir_num++;
                handle_directory(src_path, dest_path);
                break;
            case FIFO:
                fifo_num++;
                handle_fifo(src_path, dest_path);
                break;
            default:
                break;
        }
    }

    pthread_mutex_lock(&general_mutex);
    is_finished = TRUE;
    pthread_cond_broadcast(&buffer_not_empty_cond);
    pthread_mutex_unlock(&general_mutex);
    return (void *) 0;
}

void *worker(void *arg) {
    while (TRUE) {
        pthread_mutex_lock(&general_mutex);
        while (!is_finished && buffer[0].src_fd == -1) {
            pthread_cond_wait(&buffer_not_empty_cond, &general_mutex);
        }

        if (buffer[0].src_fd != -1) {
            curr_occupancy--;
            available_workers--;
        } else {
            pthread_mutex_unlock(&general_mutex);

            if (is_finished == TRUE) {
                break;
            }

            continue;
        }

        CopyInfo copy_info = buffer[0];

        for (int i = 0; i < buffer_size - 1; i++) {
            buffer[i] = buffer[i + 1];
        }

        buffer[buffer_size - 1].src_fd = -1;

        pthread_mutex_unlock(&general_mutex);

        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;

        while ((bytes_read = read(copy_info.src_fd, buffer, BUFFER_SIZE)) > 0) {
            if (write(copy_info.dest_fd, buffer, bytes_read) != bytes_read) {
                display_error("Error writing to the destination file %s", copy_info.dest);
                break;
            }

            pthread_mutex_lock(&general_mutex);
            total_bytes += bytes_read;
            pthread_mutex_unlock(&general_mutex);
        }

        if (bytes_read == -1) {
            display_error("Error reading from the source file %s", copy_info.src);
        } else {
            my_printf("Copied %s to %s\n", copy_info.src, copy_info.dest);
        }

        if (close(copy_info.src_fd) == -1) {
            display_error("Error closing the source file %s", copy_info.src);
        }

        if (close(copy_info.dest_fd) == -1) {
            display_error("Error closing the destination file %s", copy_info.dest);
        }

        pthread_mutex_lock(&general_mutex);
        available_workers++;
        pthread_cond_signal(&buffer_not_full_cond);
        pthread_mutex_unlock(&general_mutex);
    }

    return (void *) 0;
}

void sigint_handler(int signo) {
    pthread_mutex_lock(&general_mutex);
    is_finished = TRUE;
    pthread_cond_broadcast(&buffer_not_empty_cond);
    pthread_mutex_unlock(&general_mutex);

    pthread_join(manager_thread, NULL);
    pthread_cancel(manager_thread);

    for (int i = 0; i < worker_num; i++) {
        pthread_join(worker_threads[i], NULL);
        pthread_cancel(worker_threads[i]);
    }

    Bool is_time_printable = TRUE;
    long seconds, nanoseconds;
    double elapsed;

    if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
        display_error("Error getting the end time");
        is_time_printable = FALSE;
    }

    seconds = end.tv_sec - start.tv_sec;
    nanoseconds = end.tv_nsec - start.tv_nsec;

    if (nanoseconds < 0) {
        seconds -= 1;
        nanoseconds += 1000000000;
    }

    elapsed = seconds * 1000.0 + nanoseconds / 1000000.0;
    long minutes = (long) (elapsed / 60000);
    long secs = (long) ((elapsed - minutes * 60000) / 1000);
    long millis = (long) (elapsed - minutes * 60000 - secs * 1000);

    my_printf("\nSIGINT signal received. Printing the statistics so far and exiting...\n\n");
    my_printf("\n---------------STATISTICS--------------------\n");
    my_printf("Consumers: %d - Buffer Size: %d\n", worker_num, buffer_size);
    my_printf("Number of Regular File: %d\n", regular_num);
    my_printf("Number of FIFO File: %d\n", fifo_num);
    my_printf("Number of Directory: %d\n", dir_num);
    my_printf("Bytes copied so far: %d\n", total_bytes);

    if (is_time_printable == TRUE) {
        my_printf("Elapsed time so far: %02ld:%02ld.%03ld (min:sec.mili)\n", minutes, secs, millis);
    }

    exit(EXIT_SUCCESS);
}
#include "server.h"

int cook_thread_num;
int delivery_thread_num;
int port;
int speed;

int server_fd;
int socket_fd;

OrderQueue cooked_queue;
OrderQueue delivery_queue;
OrderQueue preparation_queue;

pthread_mutex_t apparatus_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cooked_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t delivery_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t finished_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t oven_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t oven_placing_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t oven_removing_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t preparation_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t signal_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t apparatus_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t delivery_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t finished_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t oven_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t preparation_cond = PTHREAD_COND_INITIALIZER;

int *delivery_stats;
int *cook_stats;
int working_cooks = 0;
int working_deliveries = 0;
pthread_t *cook_threads;
pthread_t *delivery_threads;

int available_apparatus = APPARATUS_NUM;
int finished_orders = 0;
int order_num = 0;
int oven_occupancy = 0;

timer_t manager_timer_id;
Bool finished = FALSE;
volatile sig_atomic_t signal_received = FALSE;
volatile sig_atomic_t sigint_received = FALSE;
volatile sig_atomic_t sigpipe_count = 0;

int main(int argc, char const *argv[]) {
    if (argc != 5) {
        my_printf("Usage: %s <portNumber> <cookThreadPoolSize> <deliveryPoolSize> <k>\n", argv[0]);
        return 1;
    }

    port = parse_int(argv[1]);
    cook_thread_num = parse_int(argv[2]);
    delivery_thread_num = parse_int(argv[3]);
    speed = parse_int(argv[4]);

    if (port == INT_MIN) {
        my_printf("Invalid port number. Please enter a valid integer.\n");
        return 1;
    }

    if (cook_thread_num == INT_MIN) {
        my_printf("Invalid cook thread pool size. Please enter a valid integer.\n");
        return 1;
    }

    if (delivery_thread_num == INT_MIN) {
        my_printf("Invalid delivery thread pool size. Please enter a valid integer.\n");
        return 1;
    }

    if (speed == INT_MIN) {
        my_printf("Invalid speed. Please enter a valid integer.\n");
        return 1;
    }

    if (assign_signal_handler(SIGINT, signal_handler) == FAILURE) {
        my_perror("Error registering SIGINT handler for server");
        return -1;
    }

    if (assign_signal_handler(SIGPIPE, signal_handler) == FAILURE) {
        my_perror("Error registering SIGPIPE handler for server");
        return -1;
    }

    if (assign_signal_handler(SIGTERM, sigterm_handler) == FAILURE) {
        my_perror("Error registering SIGTERM handler for server");
        return -1;
    }

    if (atexit(cleanup) != 0) {
        my_perror("Error registering cleanup function");
        return -1;
    }

    preparation_queue.size = 0;
    preparation_queue.capacity = cook_thread_num * 2;
    preparation_queue.entries = (Order *) calloc(cook_thread_num * 2, sizeof(Order));

    delivery_queue.size = 0;
    delivery_queue.capacity = delivery_thread_num * 2;
    delivery_queue.entries = (Order *) calloc(delivery_thread_num * 2, sizeof(Order));

    cooked_queue.size = 0;
    cooked_queue.capacity = cook_thread_num * 2;
    cooked_queue.entries = (Order *) calloc(cook_thread_num * 2, sizeof(Order));

    cook_threads = (pthread_t *) calloc(cook_thread_num, sizeof(pthread_t));
    delivery_threads = (pthread_t *) calloc(delivery_thread_num, sizeof(pthread_t));
    delivery_stats = (int *) calloc(delivery_thread_num, sizeof(int));
    cook_stats = (int *) calloc(cook_thread_num, sizeof(int));

    if (preparation_queue.entries == NULL) {
        my_perror("Error allocating memory for preparation queue");
        return -1;
    }

    if (delivery_queue.entries == NULL) {
        my_perror("Error allocating memory for delivery queue");
        return -1;
    }

    if (cook_threads == NULL) {
        my_perror("Error allocating memory for cook threads");
        return -1;
    }

    if (delivery_threads == NULL) {
        my_perror("Error allocating memory for delivery threads");
        return -1;
    }

    for (int i = 0; i < cook_thread_num; i++) {
        int *id = (int *) calloc(1, sizeof(int));
        *id = i + 1;

        if (pthread_create(&cook_threads[i], NULL, cook, id) != 0) {
            my_perror("Error creating cook thread");
            return -1;
        }
    }

    for (int i = 0; i < delivery_thread_num; i++) {
        int *id = (int *) calloc(1, sizeof(int));
        *id = i + 1;

        if (pthread_create(&delivery_threads[i], NULL, delivery_personel, id) != 0) {
            my_perror("Error creating delivery thread");
            return -1;
        }
    }

    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        my_perror("An error occurred while creating the socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        my_perror("An error occurred while setting socket options");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        my_perror("An error occurred while binding the socket");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        my_perror("An error occurred while listening on the socket");
        exit(EXIT_FAILURE);
    }

    if (init_log() == FAILURE) {
        exit(EXIT_FAILURE);
    }

    if (log_message(INFO, "Pide shop started on IP address %s:%d with %d cooks and %d delivery personel", "127.0.0.1",
                    port, cook_thread_num, delivery_thread_num) == FAILURE) {
        exit(EXIT_FAILURE);
    }

    Order order;
    pid_t client_pid;

    struct sigevent sev;
    struct itimerspec its;
    memset(&sev, 0, sizeof(struct sigevent));

    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = manager_timer_handler;
    timer_create(CLOCK_REALTIME, &sev, &manager_timer_id);

    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 1;
    its.it_interval.tv_nsec = 0;
    timer_settime(manager_timer_id, 0, &its, NULL);

    my_printf("> PideShop active waiting for connection\n");

    while (TRUE) {
        if ((socket_fd = accept(server_fd, (struct sockaddr *) &address, &addrlen)) < 0) {
            my_perror("An error occurred while accepting the connection");
            exit(EXIT_FAILURE);
        }

        while (TRUE) {
            if (read(socket_fd, &order, sizeof(Order)) <= 0) {
                kill(getpid(), SIGPIPE);
                break;
            }

            if (order.status == DONE) {
                my_printf("> %d new customers... Serving\n", order_num);
                break;
            }

            log_message(INFO, "Order #%d received from PID %ld with destination (%d, %d)", order.id, (long) order.pid,
                        order.destination.x, order.destination.y);
            pthread_mutex_lock(&preparation_mutex);
            enqueue(&preparation_queue, order);
            pthread_mutex_unlock(&preparation_mutex);
            client_pid = order.pid;
            order_num++;
        }

        for (int i = 0; i < preparation_queue.size; i++) {
            while (working_cooks == cook_thread_num) {
                pthread_cond_wait(&preparation_cond, &preparation_mutex);

                if (is_signal_received()) {
                    pthread_mutex_unlock(&preparation_mutex);
                    break;
                }
            }

            working_cooks++;
            pthread_cond_signal(&preparation_cond);
            pthread_mutex_unlock(&preparation_mutex);
        }

        if (is_signal_received()) {
            break;
        }

        pthread_mutex_lock(&finished_mutex);
        while (!finished) {
            pthread_cond_wait(&finished_cond, &finished_mutex);

            if (is_signal_received()) {
                pthread_mutex_unlock(&finished_mutex);
                break;
            }
        }

        finished = FALSE;
        pthread_mutex_unlock(&finished_mutex);

        order_num = 0;
        cooked_queue.size = 0;
        delivery_queue.size = 0;
        preparation_queue.size = 0;
        working_cooks = 0;
        working_deliveries = 0;
        finished_orders = 0;

        order.status = DONE;
        write(socket_fd, &order, sizeof(Order));

        int max_delivery = delivery_stats[0];
        int max_delivery_id = 1;

        for (int i = 1; i < delivery_thread_num; i++) {
            if (delivery_stats[i] > max_delivery) {
                max_delivery = delivery_stats[i];
                max_delivery_id = i + 1;
            }
        }

        int max_cook = cook_stats[0];
        int max_cook_id = 1;

        for (int i = 1; i < cook_thread_num; i++) {
            if (cook_stats[i] > max_cook) {
                max_cook = cook_stats[i];
                max_cook_id = i + 1;
            }
        }

        my_printf("> Done serving client @ XXX PID %ld\n", (long) client_pid);
        my_printf("> Thanks Cook %d and Moto %d\n", max_cook_id, max_delivery_id);
        my_printf("> Active waiting for connections\n");
        log_message(INFO, "All orders for PID %ld are delivered.", (long) client_pid);
        log_message(INFO, "Delivery personel #%d is the most productive with %d orders.", max_delivery_id,
                    max_delivery);
        log_message(INFO, "Cook #%d is the most productive with %d orders.", max_cook_id, max_cook);

        memset(cooked_queue.entries, 0, cooked_queue.capacity * sizeof(Order));
        memset(delivery_queue.entries, 0, delivery_queue.capacity * sizeof(Order));
        memset(preparation_queue.entries, 0, preparation_queue.capacity * sizeof(Order));
        memset(delivery_stats, 0, delivery_thread_num * sizeof(int));
        memset(cook_stats, 0, cook_thread_num * sizeof(int));
    }

    return 0;
}

Bool is_signal_received(void) {
    pthread_mutex_lock(&signal_mutex);
    Bool result = signal_received;
    pthread_mutex_unlock(&signal_mutex);
    return result;
}

Order dequeue(OrderQueue *order_queue) {
    Order order = order_queue->entries[0];

    for (int i = 1; i < order_queue->size; i++) {
        order_queue->entries[i - 1] = order_queue->entries[i];
    }

    order_queue->size--;
    return order;
}

void cleanup(void) {
    timer_delete(manager_timer_id);
    close(server_fd);
    close(socket_fd);

    free(cook_threads);
    free(cooked_queue.entries);
    free(delivery_queue.entries);
    free(delivery_stats);
    free(cook_stats);
    free(delivery_threads);
    free(preparation_queue.entries);

    pthread_mutex_destroy(&apparatus_mutex);
    pthread_mutex_destroy(&cooked_mutex);
    pthread_mutex_destroy(&delivery_mutex);
    pthread_mutex_destroy(&finished_mutex);
    pthread_mutex_destroy(&oven_mutex);
    pthread_mutex_destroy(&oven_placing_mutex);
    pthread_mutex_destroy(&oven_removing_mutex);
    pthread_mutex_destroy(&preparation_mutex);
    pthread_mutex_destroy(&signal_mutex);

    pthread_cond_destroy(&apparatus_cond);
    pthread_cond_destroy(&delivery_cond);
    pthread_cond_destroy(&oven_cond);
    pthread_cond_destroy(&preparation_cond);
}

void *cook(void *arg) {
    int id = *((int *) arg);
    free(arg);

    while (TRUE) {
        // Check if SIGINT is received
        if (is_signal_received()) {
            break;
        }

        // Get the order from the preparation queue
        pthread_mutex_lock(&preparation_mutex);
        while (preparation_queue.size == 0) {
            pthread_cond_wait(&preparation_cond, &preparation_mutex);

            if (is_signal_received()) {
                pthread_mutex_unlock(&preparation_mutex);
                break;
            }
        }

        Order order = dequeue(&preparation_queue);
        pthread_mutex_unlock(&preparation_mutex);

        order.cook_id = id;
        order.status = PREPARING;
        write(socket_fd, &order, sizeof(Order));
        if (order.id != 0) {
            log_message(INFO, "Order #%d from PID %ld is being prepared by cook #%d", order.id, (long) order.pid, id);
        }

        clock_t t;
        double complex A[M][N];
        double complex A_pinv[N][M];

        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                A[i][j] = i + j * I;
            }
        }

        t = clock();
        calculate_pseudo_inverse(A, A_pinv);
        t = clock() - t;
        double preparation_time = ((double) t) / CLOCKS_PER_SEC; // in seconds

        order.status = PREPARED;
        write(socket_fd, &order, sizeof(Order));
        if (order.id != 0) {
            log_message(INFO, "Order #%d from PID %ld is prepared by cook #%d", order.id, (long) order.pid, id);
        }

        if (is_signal_received()) {
            break;
        }

        // Check if the oven is available
        pthread_mutex_lock(&oven_mutex);
        while (oven_occupancy == OVEN_CAPACITY) {
            pthread_cond_wait(&oven_cond, &oven_mutex);

            if (is_signal_received()) {
                pthread_mutex_unlock(&oven_mutex);
                break;
            }
        }

        oven_occupancy++;
        pthread_mutex_unlock(&oven_mutex);

        if (is_signal_received()) {
            break;
        }

        // Check if the apparatus is available
        pthread_mutex_lock(&apparatus_mutex);
        while (available_apparatus == 0) {
            pthread_cond_wait(&apparatus_cond, &apparatus_mutex);

            if (is_signal_received()) {
                pthread_mutex_unlock(&apparatus_mutex);
                break;
            }
        }

        available_apparatus--;
        pthread_mutex_unlock(&apparatus_mutex);

        // Place the order in the oven and start the timer
        pthread_mutex_lock(&oven_placing_mutex);
        timer_t timer_id;
        struct sigevent sev;
        struct itimerspec its;
        memset(&sev, 0, sizeof(struct sigevent));

        Order *timer_order = (Order *) calloc(1, sizeof(Order));
        *timer_order = order;

        order.status = COOKING;
        sev.sigev_notify = SIGEV_THREAD;
        sev.sigev_value.sival_ptr = timer_order;
        sev.sigev_notify_function = cook_timer_handler;
        timer_create(CLOCK_REALTIME, &sev, &timer_id);

        its.it_value.tv_sec = (int) (preparation_time / 2);
        its.it_value.tv_nsec = (preparation_time / 2 - its.it_value.tv_sec) * 1e9;
        its.it_interval.tv_sec = 0;
        its.it_interval.tv_nsec = 0;
        timer_settime(timer_id, 0, &its, NULL);
        pthread_mutex_unlock(&oven_placing_mutex);

        write(socket_fd, &order, sizeof(Order));
        log_message(INFO, "Order #%d from PID %ld is being cooked by cook #%d in the oven for %.4f seconds", order.id,
                    (long) order.pid, id, preparation_time / 2);

        // Put the apparatus back
        pthread_mutex_lock(&apparatus_mutex);
        available_apparatus++;
        pthread_cond_signal(&apparatus_cond);
        pthread_mutex_unlock(&apparatus_mutex);

        pthread_mutex_lock(&preparation_mutex);
        working_cooks--;
        cook_stats[id - 1] += 1;
        pthread_cond_signal(&preparation_cond);
        pthread_mutex_unlock(&preparation_mutex);
    }

    return NULL;
}

void cook_timer_handler(union sigval sv) {
    Order order = *((Order *) sv.sival_ptr);
    free(sv.sival_ptr);

    if (is_signal_received()) {
        return;
    }

    // Check if the apparatus is available
    pthread_mutex_lock(&apparatus_mutex);
    while (available_apparatus == 0) {
        pthread_cond_wait(&apparatus_cond, &apparatus_mutex);

        if (is_signal_received()) {
            pthread_mutex_unlock(&apparatus_mutex);
            break;
        }
    }
    available_apparatus--;
    pthread_mutex_unlock(&apparatus_mutex);

    // Remove the order from the oven
    pthread_mutex_lock(&oven_removing_mutex);
    pthread_mutex_lock(&oven_mutex);
    oven_occupancy--;
    pthread_cond_signal(&oven_cond);
    pthread_mutex_unlock(&oven_mutex);
    pthread_mutex_unlock(&oven_removing_mutex);

    order.status = COOKED;
    write(socket_fd, &order, sizeof(Order));
    log_message(INFO, "Order #%d from PID %ld is cooked by cook #%d", order.id, (long) order.pid, order.cook_id);

    // Put the apparatus back
    pthread_mutex_lock(&apparatus_mutex);
    available_apparatus++;
    pthread_cond_signal(&apparatus_cond);
    pthread_mutex_unlock(&apparatus_mutex);

    // Move the order to the cooked queue
    pthread_mutex_lock(&cooked_mutex);
    enqueue(&cooked_queue, order);
    pthread_mutex_unlock(&cooked_mutex);
}

void *delivery_personel(void *arg) {
    int id = *((int *) arg);
    free(arg);

    while (TRUE) {
        if (is_signal_received()) {
            break;
        }

        pthread_mutex_lock(&delivery_mutex);
        while (delivery_queue.size == 0) {
            pthread_cond_wait(&delivery_cond, &delivery_mutex);

            if (is_signal_received()) {
                pthread_mutex_unlock(&delivery_mutex);
                break;
            }
        }

        Order order = dequeue(&delivery_queue);
        pthread_mutex_unlock(&delivery_mutex);

        Coordinate start = {order.area_end.x / 2, order.area_end.y / 2};
        order.delivery_id = id;
        deliver_order(order, start, order.destination);
        start = order.destination;

        int distance = abs(start.x - order.area_end.x / 2) + abs(start.y - order.area_end.y / 2);
        int duration = distance / speed;

        if (is_signal_received()) {
            break;
        }

        sleep(duration);

        if (is_signal_received()) {
            break;
        }

        log_message(INFO, "Delivery personel #%d has returned to the pide shop", id);

        pthread_mutex_lock(&finished_mutex);
        if (++finished_orders == order_num) {
            finished = TRUE;
            pthread_cond_signal(&finished_cond);
        }
        pthread_mutex_unlock(&finished_mutex);

        pthread_mutex_lock(&delivery_mutex);
        working_deliveries--;
        delivery_stats[id - 1] += 1;
        pthread_cond_signal(&delivery_cond);
        pthread_mutex_unlock(&delivery_mutex);
    }

    return NULL;
}

void deliver_order(Order order, Coordinate start, Coordinate destination) {
    int distance = abs(start.x - destination.x) + abs(start.y - destination.y);
    int duration = distance / speed;

    order.status = DELIVERING;
    write(socket_fd, &order, sizeof(Order));
    if (order.id != 0) {
        log_message(INFO, "Order #%d from PID %ld is being delivered by delivery personel #%d", order.id,
                    (long) order.pid, order.delivery_id);
    }

    if (is_signal_received()) {
        return;
    }

    sleep(duration);

    if (is_signal_received()) {
        return;
    }

    order.status = DELIVERED;
    write(socket_fd, &order, sizeof(Order));
    log_message(INFO, "Order #%d from PID %ld is delivered by delivery personel #%d", order.id, (long) order.pid,
                order.delivery_id);
}

void enqueue(OrderQueue *queue, Order order) {
    if (queue->capacity == queue->size) {
        queue->capacity *= 2;
        queue->entries = (Order *) realloc(queue->entries, queue->capacity * sizeof(Order));
    }

    queue->entries[queue->size++] = order;
}

void manager_timer_handler(union sigval sv) {
    if (is_signal_received()) {
        return;
    }

    while (cooked_queue.size > 0) {
        pthread_mutex_lock(&cooked_mutex);
        Order order = dequeue(&cooked_queue);
        pthread_mutex_unlock(&cooked_mutex);

        pthread_mutex_lock(&delivery_mutex);
        enqueue(&delivery_queue, order);

        while (working_deliveries == delivery_thread_num) {
            pthread_cond_wait(&delivery_cond, &delivery_mutex);

            if (is_signal_received()) {
                pthread_mutex_unlock(&delivery_mutex);
                break;
            }
        }

        working_deliveries++;
        pthread_cond_signal(&delivery_cond);
        pthread_mutex_unlock(&delivery_mutex);
    }
}

void signal_handler(int signo) {
    pthread_mutex_lock(&signal_mutex);
    signal_received = TRUE;
    sigpipe_count += (signo == SIGPIPE);
    pthread_mutex_unlock(&signal_mutex);

    if (signo == SIGINT) {
        sigint_received = TRUE;
        my_printf("\n> Upps quitting...\nWriting log file...\n");
        log_message(INFO, "SIGINT received. Exiting...");
    } else if (signo == SIGPIPE) {
        if (sigpipe_count == 1 && !sigint_received) {
            my_printf("> Pide shop bankrupted. Exiting...\n");
            log_message(ERROR, "SIGPIPE received. Exiting...");
            kill(getpid(), SIGTERM);
        }

        return;
    }

    pthread_cond_broadcast(&apparatus_cond);
    pthread_cond_broadcast(&delivery_cond);
    pthread_cond_broadcast(&finished_cond);
    pthread_cond_broadcast(&oven_cond);
    pthread_cond_broadcast(&preparation_cond);

    for (int i = 0; i < cook_thread_num; i++) {
        my_printf("> Cook %d is joining\n", i + 1);
        pthread_join(cook_threads[i], NULL);
    }

    for (int i = 0; i < delivery_thread_num; i++) {
        my_printf("> Delivery %d is joining\n", i + 1);
        pthread_join(delivery_threads[i], NULL);
    }

    exit(EXIT_SUCCESS);
}

void sigterm_handler(int signo) {
    pthread_cond_broadcast(&apparatus_cond);
    pthread_cond_broadcast(&delivery_cond);
    pthread_cond_broadcast(&oven_cond);
    pthread_cond_broadcast(&preparation_cond);

    for (int i = 0; i < cook_thread_num; i++) {
        my_printf("> Cook %d is joining\n", i + 1);
        pthread_join(cook_threads[i], NULL);
    }

    for (int i = 0; i < delivery_thread_num; i++) {
        my_printf("> Delivery %d is joining\n", i + 1);
        pthread_join(delivery_threads[i], NULL);
    }

    exit(EXIT_SUCCESS);
}

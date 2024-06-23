#include "client.h"

const char *status_names[] = {
    [PLACED] = "Placed", [PREPARING] = "Preparing",   [PREPARED] = "Prepared",  [COOKING] = "Cooking",
    [COOKED] = "Cooked", [DELIVERING] = "Delivering", [DELIVERED] = "Delivered"};

const char *ip;
int port;
int client_num;
int p;
int q;

int socket_fd;
int client_fd;

int main(int argc, char const *argv[]) {
    if (argc != 6) {
        my_printf("Usage: %s <ipAddress> <portNumber> <numberOfClients> <p> <q>\n", argv[0]);
        return 1;
    }

    ip = argv[1];
    port = parse_int(argv[2]);
    client_num = parse_int(argv[3]);
    p = parse_int(argv[4]);
    q = parse_int(argv[5]);

    if (is_valid_ip(ip) == FALSE) {
        my_printf("Invalid IP address. Please enter a valid IP address.\n");
        return 1;
    }

    if (port == INT_MIN) {
        my_printf("Invalid port number. Please enter a valid integer.\n");
        return 1;
    }

    if (client_num == INT_MIN) {
        my_printf("Invalid number of clients. Please enter a valid integer.\n");
        return 1;
    }

    if (p == INT_MIN) {
        my_printf("Invalid p. Please enter a valid integer.\n");
        return 1;
    }

    if (q == INT_MIN) {
        my_printf("Invalid q. Please enter a valid integer.\n");
        return 1;
    }

    if (assign_signal_handler(SIGINT, sigint_handler) == FAILURE) {
        my_perror("Error registering SIGINT handler for server");
        return -1;
    }

    if (assign_signal_handler(SIGPIPE, sigpipe_handler) == FAILURE) {
        my_perror("Error registering SIGPIPE handler for server");
        return -1;
    }

    if (atexit(cleanup) != 0) {
        my_perror("Error registering cleanup function");
        return -1;
    }

    struct sockaddr_in serv_addr;

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        my_perror("An error occurred while creating the socket");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) == -1) {
        my_perror("An error occurred while converting the IP address");
        return -1;
    }

    if ((socket_fd = connect(client_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) == -1) {
        my_perror("Connection failed");
        return -1;
    }

    my_printf("> PID %ld\n", (long) getpid());
    my_printf("> Connected to the server at %s:%d\n", ip, port);
    srand(time(NULL));

    Order order;

    for (int i = 0; i < client_num; i++) {
        memset(&order, 0, sizeof(Order));
        order = generate_order(p, q);
        order.id = i + 1;
        write(client_fd, &order, sizeof(Order));
        my_printf("> Status of Order #%d with destination (%d, %d): Placed\n", order.id, order.destination.x,
                  order.destination.y);
    }

    memset(&order, 0, sizeof(Order));
    order.status = DONE;
    write(client_fd, &order, sizeof(Order));

    while (TRUE) {
        memset(&order, 0, sizeof(Order));

        if (read(client_fd, &order, sizeof(Order)) <= 0) {
            kill(getpid(), SIGPIPE);
            break;
        }

        if (order.status == DONE) {
            break;
        }

        my_printf("> Status of Order #%d with destination (%d, %d): %s\n", order.id, order.destination.x,
                  order.destination.y, status_names[order.status]);
    }

    my_printf("> All customers served\n");
    my_printf("> Log file written\n");
    return 0;
}

Order generate_order(int p, int q) {
    int x = rand() % p;
    int y = rand() % q;
    Order order = {.destination = {x, y}, .area_end = {p, q}, .status = PLACED, .pid = getpid()};
    return order;
}

void cleanup(void) {
    close(client_fd);
    close(socket_fd);
}

void sigint_handler(int signo) {
    my_printf("> Signal...\n> Cancelling orders...\n> Editing log...\n");
    exit(EXIT_SUCCESS);
}

void sigpipe_handler(int signo) {
    my_printf("> Pide shop burnt down. Exiting...\n");
    exit(EXIT_FAILURE);
}

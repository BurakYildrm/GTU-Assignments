CC = gcc
CFLAGS = -Wall -pedantic-errors
LIBFLAGS = -pthread -lrt -lm
DFLAGS = -g
INCDIR = include
SRCDIR = src
LOG_EXT = log
SERVER_TARGET = PideShop
CLIENT_TARGET = HungryVeryMuch
UTIL_SOURCES = $(wildcard $(SRCDIR)/utils/*.c)
SHARED_SOURCE = $(SRCDIR)/shared.c
SERVER_SOURCES = $(SRCDIR)/server.c $(SHARED_SOURCE) $(UTIL_SOURCES) 
CLIENT_SOURCES = $(SRCDIR)/client.c $(SHARED_SOURCE) $(UTIL_SOURCES)
HEADERS = $(wildcard $(INCDIR)/*.h) $(wildcard $(INCDIR)/utils/*.h)
VALGRIND_OPTIONS = --leak-check=full --show-leak-kinds=all --track-origins=yes
V_PORT =
V_COOK = 
V_DELIVERY =
V_SPEED =
V_IP =
V_CLIENT = 
V_P = 
V_Q =

export V_PORT
export V_COOK
export V_DELIVERY
export V_SPEED
export V_IP
export V_CLIENT
export V_P
export V_Q

all: server client

server: $(SERVER_SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(SERVER_TARGET) $(SERVER_SOURCES) -I$(INCDIR) $(LIBFLAGS)

client: $(CLIENT_SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(CLIENT_TARGET) $(CLIENT_SOURCES) -I$(INCDIR) $(LIBFLAGS)

debug_server:
	$(CC) $(CFLAGS) $(DFLAGS) -o $(SERVER_TARGET) $(SERVER_SOURCES) -I$(INCDIR) $(LIBFLAGS)

debug_client:
	$(CC) $(CFLAGS) $(DFLAGS) -o $(CLIENT_TARGET) $(CLIENT_SOURCES) -I$(INCDIR) $(LIBFLAGS)

clean:
	rm -f $(SERVER_TARGET) $(CLIENT_TARGET) *.$(LOG_EXT)

valgrind_server: debug_server
	valgrind $(VALGRIND_OPTIONS) ./$(SERVER_TARGET) $(V_PORT) $(V_COOK) $(V_DELIVERY) $(V_SPEED)

valgrind_client: debug_client
	valgrind $(VALGRIND_OPTIONS) ./$(CLIENT_TARGET) $(V_IP) $(V_PORT) $(V_CLIENT) $(V_P) $(V_Q)

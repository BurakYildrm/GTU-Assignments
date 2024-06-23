CC = gcc
CFLAGS = -Wall -std=c99
IPCFLAGS = -pthread -lrt
DFLAGS = -g
INCDIR = include
SRCDIR = src
LOG_EXT = log
LOCK_EXT = dirlock
SERVER_TARGET = neHosServer
CLIENT_TARGET = neHosClient
UTIL_SOURCES = $(wildcard $(SRCDIR)/utils/*.c)
SHARED_SOURCE = $(SRCDIR)/shared.c
SERVER_SOURCES = $(SRCDIR)/server.c $(SHARED_SOURCE) $(UTIL_SOURCES) 
CLIENT_SOURCES = $(SRCDIR)/client.c $(SHARED_SOURCE) $(UTIL_SOURCES)
HEADERS = $(wildcard $(INCDIR)/*.h) $(wildcard $(INCDIR)/utils/*.h)
VALGRIND_OPTIONS = --leak-check=full --show-leak-kinds=all --track-origins=yes
V_DIR =
V_CLIENT = 
V_CONNECT =
V_PID = 

export V_DIR
export V_CLIENT
export V_CONNECT
export V_PID

OS := $(shell uname)

ifeq ($(OS), Darwin)
	IPCFLAGS =
endif

all: server client

server: $(SERVER_SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(SERVER_TARGET) $(SERVER_SOURCES) -I$(INCDIR) $(IPCFLAGS)

client: $(CLIENT_SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(CLIENT_TARGET) $(CLIENT_SOURCES) -I$(INCDIR) $(IPCFLAGS)

debug_server:
	$(CC) $(CFLAGS) $(DFLAGS) -o $(SERVER_TARGET) $(SERVER_SOURCES) -I$(INCDIR) $(IPCFLAGS)

debug_client:
	$(CC) $(CFLAGS) $(DFLAGS) -o $(CLIENT_TARGET) $(CLIENT_SOURCES) -I$(INCDIR) $(IPCFLAGS)

clean:
	rm -f $(SERVER_TARGET) $(CLIENT_TARGET) *.$(LOG_EXT) .*.$(LOCK_EXT)

valgrind_server: debug_server
	valgrind $(VALGRIND_OPTIONS) ./$(SERVER_TARGET) $(V_DIR) $(V_CLIENT)

valgrind_client: debug_client
	valgrind $(VALGRIND_OPTIONS) ./$(CLIENT_TARGET) $(V_CONNECT) $(V_PID)
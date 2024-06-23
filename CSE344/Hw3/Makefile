CC = gcc
CFLAGS = -w
DFLAGS = -g
TFLAGS = -pthread
INCDIR = include
SRCDIR = src
TARGET = main
SOURCES = $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/**/*.c)
HEADERS = $(wildcard $(INCDIR)/*.h) $(wildcard $(INCDIR)/**/*.h)
VALGRIND_MEMORY_OPTIONS = --leak-check=full --show-leak-kinds=all --track-origins=yes
VALGRIND_THREAD_OPTIONS = --tool=helgrind

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) -I$(INCDIR) $(TFLAGS)

debug: CFLAGS += $(DFLAGS)
debug: $(TARGET)

clean:
	rm -f $(TARGET)

valgrind_memory: debug
	valgrind $(VALGRIND_MEMORY_OPTIONS) ./$(TARGET)

valgrind_thread: debug
	valgrind $(VALGRIND_THREAD_OPTIONS) ./$(TARGET)
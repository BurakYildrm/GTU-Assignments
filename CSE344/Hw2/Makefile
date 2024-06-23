CC = gcc
CFLAGS = -w
DFLAGS = -g
INCDIR = include
SRCDIR = src
TARGET = main
SOURCES = $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/**/*.c)
HEADERS = $(wildcard $(INCDIR)/*.h) $(wildcard $(INCDIR)/**/*.h)
VALGRIND_OPTIONS = --leak-check=full --show-leak-kinds=all --track-origins=yes

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) -I$(INCDIR)

debug: CFLAGS += $(DFLAGS)
debug: $(TARGET)

clean:
	rm -f $(TARGET) fifo_*

valgrind: debug
	valgrind $(VALGRIND_OPTIONS) ./$(TARGET)
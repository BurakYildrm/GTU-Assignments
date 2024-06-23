CC = gcc
CFLAGS = -w
DFLAGS = -g
TFLAGS = -pthread
UTILS_SRCDIR = utils/src
UTILS_INCDIR = utils/include
TARGET = MWCp
SOURCES = ./1901042609_main.c $(wildcard $(UTILS_SRCDIR)/*.c)
HEADERS = ./1901042609_main.h $(wildcard $(UTILS_INCDIR)/**/*.h)
INCDIRS = -I. -I$(UTILS_INCDIR)
V_BUFFER =
V_WORKER = 
V_SRC =
V_DEST = 

export V_BUFFER
export V_WORKER
export V_SRC
export V_DEST

OS := $(shell uname)

ifeq ($(OS), Darwin)
	TFLAGS =
endif

all: $(TARGET)

test: $(TARGET)
	./test.sh $(V_BUFFER) $(V_WORKER) $(V_SRC) $(V_DEST)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(INCDIRS) $(TFLAGS)

debug: CFLAGS += $(DFLAGS)
debug: $(TARGET)

clean:
	rm -rf $(TARGET)

valgrind_memory: debug
	valgrind  ./$(TARGET) $(V_BUFFER) $(V_WORKER) $(V_SRC) $(V_DEST)

valgrind_thread: debug
	valgrind --tool=helgrind ./$(TARGET) $(V_BUFFER) $(V_WORKER) $(V_SRC) $(V_DEST)
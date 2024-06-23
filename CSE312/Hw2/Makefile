CC = g++
FLAGS = -Wall -std=c++11
INCDIR = include
SRCDIR = src
TARGET_MAKE_FS = makeFileSystem
TARGET_FS_OP = fileSystemOper
SHARED_SOURCES = $(SRCDIR)/fileSystem.cpp $(SRCDIR)/utils.cpp
SHARED_HEADERS = $(INCDIR)/fileSystem.hpp $(INCDIR)/utils.hpp

all: $(TARGET_MAKE_FS) $(TARGET_FS_OP)

$(TARGET_MAKE_FS): $(SHARED_SOURCES) $(SRCDIR)/makeFileSystem.cpp $(SHARED_HEADERS)
	$(CC) $(FLAGS) -I$(INCDIR) -o $@ $(SHARED_SOURCES) $(SRCDIR)/makeFileSystem.cpp

$(TARGET_FS_OP): $(SHARED_SOURCES) $(SRCDIR)/fileSystemOper.cpp $(SHARED_HEADERS)
	$(CC) $(FLAGS) -I$(INCDIR) -o $@ $(SHARED_SOURCES) $(SRCDIR)/fileSystemOper.cpp

clean:
	rm -f $(TARGET_MAKE_FS) $(TARGET_FS_OP)
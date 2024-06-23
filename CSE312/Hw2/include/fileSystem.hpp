#ifndef FILE_SYSTEM_HPP
#define FILE_SYSTEM_HPP

#include <array>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <ios>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "utils.hpp"

#define CORNER_LEFT_DOWN "\u2514"
#define CORNER_LEFT_UP "\u250C"
#define CORNER_RIGHT_DOWN "\u2518"
#define CORNER_RIGHT_UP "\u2510"
#define CROSS "\u253C"
#define HORIZONTAL_LINE "\u2500"
#define T_DOWN "\u2534"
#define T_LEFT "\u251C"
#define T_RIGHT "\u2524"
#define T_UP "\u252C"
#define VERTICAL_LINE "\u2502"

#define KB 1024
#define MB 1024 * KB
#define BLOCK_NUMBER 4096
#define OPERATION_NUMBER 9
#define MAX_FILENAME 8
#define MAX_EXTENSION 3
#define MAX_PASSWORD 6
#define ROOT_DIRECTORY "\\"

using namespace std;

typedef enum {
    READ_ONLY = 0x01,
    WRITE_ONLY = 0x02,
    READ_WRITE = 0x03,
    DIRECTORY = 0x04,
    PASSWORD = 0x08,
} FileAttribute;

typedef enum {
    END_OF_FILE = -1,
    FREE = -2,
} FATEntryStatus;

typedef enum {
    LIST,
    CREATE_DIRECTORY,
    REMOVE_DIRECTORY,
    PRINT_INFO,
    WRITE_FILE,
    READ_FILE,
    DELETE_FILE,
    CHANGE_PERMISSION,
    ADD_PASSWORD,
    UNKNOWN,
} FileSystemOperation;

typedef enum {
    FS_OPEN_FAILED,
    FS_READ_FAILED,
    FS_WRITE_FAILED,
    FS_CREATE_FAILED,
    FNAME_TOO_LONG,
    FILE_NOT_FOUND,
    FILE_EXISTS,
    PASSWORD_REQUIRED,
    WRONG_PASSWORD,
    UNKNOWN_OPERATION,
    INVALID_ARGUMENTS,
    INVALID_PATH,
    NOT_ENOUGH_SPACE,
    FILE_READ_FAILED,
    NO_ERROR
} FileSystemError;

typedef struct {
    string name;
    int argc;
} OperationInfo;

typedef struct {
    string filename;
    int16_t blockNum;
} BlockInfo;

struct DirectoryEntry {
    array<uint8_t, MAX_FILENAME> filename;   // 8 bytes
    array<uint8_t, MAX_EXTENSION> extension; // 3 bytes
    uint8_t attributes;                      // 1 byte
    array<uint8_t, MAX_PASSWORD> password;   // 6 bytes
    uint16_t firstBlock;                     // 2 bytes
    uint32_t creationTime;                   // 4 bytes
    uint32_t lastModifiedTime;               // 4 bytes
    uint32_t size;                           // 4 bytes

    DirectoryEntry() : filename{0}, extension{0}, attributes(0), password{0}, firstBlock(0), creationTime(0), lastModifiedTime(0), size(0) {}
} __attribute__((packed));

struct SuperBlock {
    uint16_t blockSize;           // 2 bytes (Block size in bytes)
    uint16_t blockCount;          // 2 bytes
    uint16_t fileCount;           // 2 bytes
    uint16_t directoryCount;      // 2 bytes
    uint16_t freeBlocks;          // 2 bytes
    uint16_t fatFirstBlock;       // 2 bytes
    uint16_t maxEntryPerBlock;    // 2 bytes
    DirectoryEntry rootDirectory; // 32 bytes
} __attribute__((packed));

class FileSystem {
  private:
    string fsName;
    SuperBlock superBlock;
    array<int16_t, BLOCK_NUMBER> fat;
    unique_ptr<uint8_t[]> fsBlocks;

    // file attribute checkers
    bool isPasswordProtected(const uint8_t attributes) { return (attributes & PASSWORD) != 0; }
    bool isDirectory(const uint8_t attributes) { return (attributes & DIRECTORY) != 0; }
    bool isReadable(const uint8_t attributes) { return (attributes & READ_ONLY) != 0; }
    bool isWritable(const uint8_t attributes) { return (attributes & WRITE_ONLY) != 0; }

    // helper functions
    DirectoryEntry *addDirectoryEntry(const string &path, DirectoryEntry *parentEntry, const bool isDir);
    DirectoryEntry *getDirectoryEntry(const string &path);
    DirectoryEntry *getParentDirectoryEntry(const string &path);
    bool checkPassword(DirectoryEntry *entry, const string &password, const string &path, const FileSystemOperation &operation);
    bool copyFile(ifstream &src, DirectoryEntry *dest);
    bool isBlockEmpty(const int16_t block);
    bool isRootDirectory(const string &path);
    int16_t findFreeBlock();
    pair<string, string> splitFilename(const string &fullname);
    string attributesToString(const uint8_t attributes);
    string fullFilename(array<uint8_t, MAX_FILENAME> filename, array<uint8_t, MAX_EXTENSION> extension = array<uint8_t, MAX_EXTENSION>());
    string getRootFilename();
    void freeBlock(const int16_t block);
    void freeRelatedBlocks(const uint16_t firstBlock);
    void generateBlockMapping(vector<BlockInfo> &blockMapping, const int16_t firstBlock, const string &filename, const bool isDir);
    void initializeDirectoryEntry(DirectoryEntry *entry, DirectoryEntry *parentEntry, const pair<string, string> &filename, const bool isDir);
    void printDirectoryEntries(const vector<DirectoryEntry> &entries);

  public:
    // Constructor and destructor
    FileSystem(const string &filename, uint16_t blockSize);
    ~FileSystem();

    // File system commands and error
    static const OperationInfo operations[OPERATION_NUMBER];
    static FileSystemError fsError;

    // file system read and write
    static FileSystem *readFileSystem(const string &filename);
    bool writeFileSystem();

    // file system operations
    static FileSystemOperation getOperation(const string &operation);
    bool validateArguments(int argc, char const *argv[]);
    void addPassword(const string &path, const string &password);
    void changePermissions(const string &path, const string &permissions, const string &password);
    void createDirectory(const string &path);
    void deleteFile(const string &path, const string &password);
    void printFileSystemInfo();
    void listDirectoryEntry(const string &path, const string &password);
    void readFile(const string &src, const string &dest, const string &password);
    void removeDirectory(const string &path);
    void writeFile(const string &dest, const string &src, const string &password);
};

#endif

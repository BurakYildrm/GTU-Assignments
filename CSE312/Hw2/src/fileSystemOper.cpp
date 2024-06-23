#include <iostream>

#include "fileSystem.hpp"

int main(int argc, char const *argv[]) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <filename> <operation> <arguments>" << endl;
        return 1;
    }

    FileSystem *fs = FileSystem::readFileSystem(static_cast<string>(argv[1]));

    if (!fs) {
        switch (FileSystem::fsError) {
            case FS_OPEN_FAILED:
                cerr << "Error: File not found" << endl;
                break;
            case FS_READ_FAILED:
                cerr << "Error: Failed to read file" << endl;
                break;
            default:
                break;
        }

        return 1;
    }

    bool isValid = fs->validateArguments(argc, argv);

    if (!isValid) {
        switch (FileSystem::fsError) {
            case UNKNOWN_OPERATION:
                cerr << "Error: Unknown operation" << endl;
                break;
            case INVALID_ARGUMENTS:
                cerr << "Error: Invalid arguments" << endl;
                break;
            case INVALID_PATH:
                cerr << "Error: Invalid path" << endl;
                break;
            default:
                break;
        }

        return 1;
    }

    FileSystemOperation operation = FileSystem::getOperation(static_cast<string>(argv[2]));
    string path;

    if (operation != PRINT_INFO) {
        path = static_cast<string>(argv[3]);

        if (path.size() > 1 && path.back() == '\\') {
            path.pop_back();
        }
    }

    switch (operation) {
        case LIST:
            fs->listDirectoryEntry(path, argc == 5 ? static_cast<string>(argv[4]) : "");
            break;
        case CREATE_DIRECTORY:
            fs->createDirectory(path);
            break;
        case REMOVE_DIRECTORY:
            fs->removeDirectory(path);
            break;
        case CHANGE_PERMISSION:
            fs->changePermissions(path, static_cast<string>(argv[4]), argc == 6 ? static_cast<string>(argv[5]) : "");
            break;
        case ADD_PASSWORD:
            fs->addPassword(path, static_cast<string>(argv[4]));
            break;
        case PRINT_INFO:
            fs->printFileSystemInfo();
            break;
        case WRITE_FILE:
            fs->writeFile(path, static_cast<string>(argv[4]), argc == 6 ? static_cast<string>(argv[5]) : "");
            break;
        case READ_FILE:
            fs->readFile(path, static_cast<string>(argv[4]), argc == 6 ? static_cast<string>(argv[5]) : "");
            break;
        case DELETE_FILE:
            fs->deleteFile(path, argc == 5 ? static_cast<string>(argv[4]) : "");
            break;
        default:
            break;
    }

    fs->writeFileSystem();
    delete fs;
    return 0;
}

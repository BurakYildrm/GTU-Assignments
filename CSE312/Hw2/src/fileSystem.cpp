#include "fileSystem.hpp"

const OperationInfo FileSystem::operations[OPERATION_NUMBER] = {{"dir", 1},  {"mkdir", 1}, {"rmdir", 1}, {"dumpe2fs", 0}, {"write", 2},
                                                                {"read", 2}, {"del", 1},   {"chmod", 2}, {"addpw", 2}};
FileSystemError FileSystem::fsError = NO_ERROR;

FileSystem::FileSystem(const string &filename, uint16_t blockSize)
    : fsName(filename), fsBlocks(unique_ptr<uint8_t[]>(new uint8_t[blockSize * BLOCK_NUMBER]())) {
    uint16_t fatBlocks = BLOCK_NUMBER * sizeof(int16_t) / blockSize;
    time_t now = time(nullptr);

    superBlock.blockSize = blockSize;
    superBlock.blockCount = BLOCK_NUMBER;
    superBlock.fileCount = 0;
    superBlock.directoryCount = 1;
    superBlock.freeBlocks = BLOCK_NUMBER - fatBlocks - 2; // 1 for super block, 1 for root directory, fatBlocks for FAT
    superBlock.fatFirstBlock = 1;
    superBlock.maxEntryPerBlock = blockSize / sizeof(DirectoryEntry);
    superBlock.rootDirectory.filename = stou8Array<MAX_FILENAME>(ROOT_DIRECTORY);
    superBlock.rootDirectory.firstBlock = 1 + fatBlocks;
    superBlock.rootDirectory.attributes = DIRECTORY | READ_WRITE;
    superBlock.rootDirectory.creationTime = static_cast<uint32_t>(now);
    superBlock.rootDirectory.lastModifiedTime = static_cast<uint32_t>(now);
    superBlock.rootDirectory.size = 2 * sizeof(DirectoryEntry);

    for (int i = 0; i < BLOCK_NUMBER; i++) {
        fat[i] = FREE;
    }

    fat[0] = END_OF_FILE;
    fat[superBlock.rootDirectory.firstBlock] = END_OF_FILE;

    for (int i = 1; i < fatBlocks + 1; i++) {
        if (i == fatBlocks) {
            fat[i] = END_OF_FILE;
        } else {
            fat[i] = i + 1;
        }
    }

    DirectoryEntry *dirPtr = reinterpret_cast<DirectoryEntry *>(fsBlocks.get() + superBlock.rootDirectory.firstBlock * blockSize);

    for (int i = 0; i < 2; i++) {
        dirPtr[i] = superBlock.rootDirectory;
        dirPtr[i].filename = stou8Array<MAX_FILENAME>(string(i + 1, '.'));
    }
}

FileSystem::~FileSystem() {}

FileSystem *FileSystem::readFileSystem(const string &filename) {
    ifstream file(filename, ios::binary);

    if (!file.is_open()) {
        fsError = FS_OPEN_FAILED;
        return nullptr;
    }

    SuperBlock sb;

    file.read(reinterpret_cast<char *>(&sb), sizeof(SuperBlock));
    file.seekg(0, ios::beg);

    unique_ptr<uint8_t[]> fsBlocks(new uint8_t[sb.blockSize * sb.blockCount]);
    file.read(reinterpret_cast<char *>(fsBlocks.get()), sb.blockSize * sb.blockCount);

    if (file.fail()) {
        fsError = FS_READ_FAILED;
        return nullptr;
    }

    file.close();

    FileSystem *fs = new FileSystem(filename, sb.blockSize);
    fs->superBlock = std::move(sb);
    fs->fsBlocks = std::move(fsBlocks);

    int16_t *fat_ptr = reinterpret_cast<int16_t *>(fs->fsBlocks.get() + fs->superBlock.fatFirstBlock * fs->superBlock.blockSize);
    memcpy(fs->fat.data(), fat_ptr, sizeof(fs->fat));

    return fs;
}

bool FileSystem::writeFileSystem() {
    uint32_t blockSize = superBlock.blockSize;
    uint32_t blockCount = superBlock.blockCount;

    SuperBlock *sb = reinterpret_cast<SuperBlock *>(fsBlocks.get());
    *sb = superBlock;

    int16_t *fat_ptr = reinterpret_cast<int16_t *>(fsBlocks.get() + superBlock.fatFirstBlock * blockSize);
    memcpy(fat_ptr, fat.data(), sizeof(fat));

    ofstream file(fsName, ios::binary);

    if (!file.is_open()) {
        fsError = FS_CREATE_FAILED;
        return false;
    }

    file.write(reinterpret_cast<const char *>(fsBlocks.get()), blockSize * blockCount);

    if (file.fail()) {
        fsError = FS_WRITE_FAILED;
        return false;
    }

    file.close();
    return true;
}

DirectoryEntry *FileSystem::getDirectoryEntry(const string &path) {
    istringstream iss(path);
    string token;
    vector<string> pathSegments;
    int blockNum = superBlock.rootDirectory.firstBlock;
    int maxEntryPerBlock = superBlock.maxEntryPerBlock;

    if (isRootDirectory(path)) {
        return &superBlock.rootDirectory;
    }

    while (getline(iss, token, static_cast<char>(superBlock.rootDirectory.filename[0]))) {
        if (token.empty()) {
            continue;
        }

        if (token.size() > MAX_FILENAME + (token.find(".") != string::npos ? MAX_EXTENSION + 1 : 0)) {
            fsError = FNAME_TOO_LONG;
            return nullptr;
        }

        pathSegments.push_back(token);
    }

    for (size_t i = 0; i < pathSegments.size(); i++) {
        string segment = pathSegments[i];
        bool found = false;
        bool isDotEntry = segment == "." || segment == "..";
        size_t dotPosition = segment.find(".");
        dotPosition = dotPosition == string::npos ? segment.size() : dotPosition;
        pair<string, string> fullname =
            isDotEntry ? make_pair(segment, "")
                       : make_pair(segment.substr(0, dotPosition), segment.substr(dotPosition + (dotPosition == segment.size() ? 0 : 1)));

        do {
            DirectoryEntry *dirPtr = reinterpret_cast<DirectoryEntry *>(fsBlocks.get() + blockNum * superBlock.blockSize);

            for (int j = 0; j < maxEntryPerBlock; j++) {
                if (dirPtr[j].filename == stou8Array<MAX_FILENAME>(fullname.first) &&
                    dirPtr[j].extension == stou8Array<MAX_EXTENSION>(fullname.second)) {
                    found = true;

                    if (i == pathSegments.size() - 1) {
                        return &dirPtr[j];
                    }

                    blockNum = dirPtr[j].firstBlock;
                    break;
                }
            }

            if (!found) {
                blockNum = fat[blockNum];
            }
        } while (blockNum != END_OF_FILE && !found);

        if (!found) {
            fsError = FILE_NOT_FOUND;
            break;
        }
    }

    return nullptr;
}

FileSystemOperation FileSystem::getOperation(const string &operation) {
    for (int i = 0; i < OPERATION_NUMBER; i++) {
        if (operations[i].name == operation) {
            return static_cast<FileSystemOperation>(i);
        }
    }

    return UNKNOWN;
}

bool FileSystem::validateArguments(int argc, char const *argv[]) {
    int requiredArgc = 3;
    FileSystemOperation operation = getOperation(static_cast<string>(argv[2]));

    if (operation == UNKNOWN) {
        fsError = UNKNOWN_OPERATION;
        return false;
    }

    if ((operation == PRINT_INFO && argc != 3) || (operation != PRINT_INFO && argc == 3)) {
        fsError = INVALID_ARGUMENTS;
        return false;
    }

    if (operation == PRINT_INFO) {
        return true;
    }

    string path = static_cast<string>(argv[3]);

    if (path[0] != static_cast<char>(superBlock.rootDirectory.filename[0])) {
        fsError = INVALID_PATH;
        return false;
    }

    requiredArgc += operations[operation].argc;

    if (argc == requiredArgc || (argc == requiredArgc + 1 && operation != CREATE_DIRECTORY)) {
        return true;
    }

    fsError = INVALID_ARGUMENTS;
    return false;
}

string FileSystem::attributesToString(const uint8_t attributes) {
    string str;

    str += isPasswordProtected(attributes) ? 'p' : '-';
    str += isDirectory(attributes) ? 'd' : '-';
    str += isReadable(attributes) ? 'r' : '-';
    str += isWritable(attributes) ? 'w' : '-';

    return str;
}

string FileSystem::fullFilename(array<uint8_t, MAX_FILENAME> filename, array<uint8_t, MAX_EXTENSION> extension) {
    string filenameStr = u8ArrayToString(filename);
    string extensionStr = u8ArrayToString(extension);
    return join({filenameStr, extensionStr}, ".");
}

void FileSystem::listDirectoryEntry(const string &path, const string &password) {
    DirectoryEntry *entry = getDirectoryEntry(path);
    vector<DirectoryEntry> entries;

    if (entry == nullptr) {
        cerr << "dir: " << path << ": No such file or directory" << endl;
        return;
    }

    if (!checkPassword(entry, password, path, LIST)) {
        return;
    }

    if (!isReadable(entry->attributes)) {
        cerr << "dir: " << path << ": Permission denied" << endl;
        return;
    }

    int blockNum = entry->firstBlock;
    bool isDir = isDirectory(entry->attributes);

    if (!isDir) {
        entries.push_back(*entry);
    } else {
        do {
            DirectoryEntry *dirPtr = reinterpret_cast<DirectoryEntry *>(fsBlocks.get() + blockNum * superBlock.blockSize);

            for (int i = 0; i < superBlock.maxEntryPerBlock; i++) {
                if (dirPtr[i].filename == stou8Array<MAX_FILENAME>(".") || dirPtr[i].filename == stou8Array<MAX_FILENAME>("..")) {
                    continue;
                }

                entries.push_back(dirPtr[i]);
            }

            blockNum = fat[blockNum];
        } while (blockNum != END_OF_FILE);
    }

    entries.erase(remove_if(entries.begin(), entries.end(), [](const DirectoryEntry &entry) { return entry.filename[0] == 0; }), entries.end());
    printDirectoryEntries(entries);
}

void FileSystem::printDirectoryEntries(const vector<DirectoryEntry> &entries) {
    if (entries.empty()) {
        cout << "Directory is empty" << endl;
        return;
    }

    string attrStr = "Attributes";
    string sizeStr = "Size";
    string creationStr = "Creation Date";
    string modificationStr = "Modification Date";
    string nameStr = "Name";

    int attrLen = attrStr.size() + 1;
    int sizeLen = to_string(superBlock.blockSize * superBlock.blockCount).size() + 1;
    int timeLen = formatTime(time(nullptr)).size() + 1;
    int nameLen = MAX_FILENAME + MAX_EXTENSION + 2;

    cout << endl
         << CORNER_LEFT_UP << repeat(HORIZONTAL_LINE, attrLen) << T_UP << repeat(HORIZONTAL_LINE, sizeLen) << T_UP << repeat(HORIZONTAL_LINE, timeLen)
         << T_UP << repeat(HORIZONTAL_LINE, timeLen) << T_UP << repeat(HORIZONTAL_LINE, nameLen) << CORNER_RIGHT_UP << endl;

    cout << VERTICAL_LINE << setw(attrLen) << left << attrStr << VERTICAL_LINE << setw(sizeLen) << left << sizeStr << VERTICAL_LINE << setw(timeLen)
         << left << creationStr << VERTICAL_LINE << setw(timeLen) << left << modificationStr << VERTICAL_LINE << setw(nameLen) << left << nameStr
         << VERTICAL_LINE << endl;

    cout << T_LEFT << repeat(HORIZONTAL_LINE, attrLen) << CROSS << repeat(HORIZONTAL_LINE, sizeLen) << CROSS << repeat(HORIZONTAL_LINE, timeLen)
         << CROSS << repeat(HORIZONTAL_LINE, timeLen) << CROSS << repeat(HORIZONTAL_LINE, nameLen) << T_RIGHT << endl;

    for (const auto &entry : entries) {
        cout << VERTICAL_LINE << setw(attrLen) << left << attributesToString(entry.attributes) << VERTICAL_LINE << setw(sizeLen) << entry.size
             << VERTICAL_LINE << setw(timeLen) << left << formatTime(entry.creationTime) << VERTICAL_LINE << setw(timeLen) << left
             << formatTime(entry.lastModifiedTime) << VERTICAL_LINE << setw(nameLen) << left << fullFilename(entry.filename, entry.extension)
             << VERTICAL_LINE << endl;
    }

    cout << CORNER_LEFT_DOWN << repeat(HORIZONTAL_LINE, attrLen) << T_DOWN << repeat(HORIZONTAL_LINE, sizeLen) << T_DOWN
         << repeat(HORIZONTAL_LINE, timeLen) << T_DOWN << repeat(HORIZONTAL_LINE, timeLen) << T_DOWN << repeat(HORIZONTAL_LINE, nameLen)
         << CORNER_RIGHT_DOWN << endl;
}

void FileSystem::createDirectory(const string &path) {
    if (isRootDirectory(path)) {
        cerr << "mkdir: '" << getRootFilename() << "': File exists" << endl;
        return;
    }

    DirectoryEntry *parentEntry = getParentDirectoryEntry(path);
    DirectoryEntry *entry = getDirectoryEntry(path);

    if (entry) {
        cerr << "mkdir: " << path << ": File exists" << endl;
        return;
    }

    if (!parentEntry) {
        cerr << "mkdir: " << path.substr(0, path.find_last_of(getRootFilename())) << ": No such file or directory" << endl;
        return;
    }

    if (!isDirectory(parentEntry->attributes)) {
        cerr << "mkdir: '" << path.substr(0, path.find_last_of(getRootFilename())) << "': Not a directory" << endl;
        return;
    }

    entry = addDirectoryEntry(path, parentEntry, true);

    if (!entry) {
        return;
    }
}

int16_t FileSystem::findFreeBlock() {
    for (int16_t i = 0; i < superBlock.blockCount; i++) {
        if (fat[i] == FREE) {
            return i;
        }
    }

    return -1;
}

void FileSystem::changePermissions(const string &path, const string &permissions, const string &password) {
    if (isRootDirectory(path)) {
        cerr << "chmod: '\\': Permission denied" << endl;
        return;
    }

    DirectoryEntry *entry = getDirectoryEntry(path);
    regex permissionPattern("^(?!.*r.*r)(?!.*w.*w)[+-][rwx]{1,2}$");

    if (!entry) {
        cerr << "chmod: " << path << ": No such file or directory" << endl;
        return;
    }

    if (!regex_match(permissions, permissionPattern)) {
        cerr << "chmod: " << permissions << ": Invalid permission" << endl;
        return;
    }

    if (!checkPassword(entry, password, path, CHANGE_PERMISSION)) {
        return;
    }

    bool isInclusive = permissions[0] == '+';

    for (auto &bit : permissions.substr(1)) {
        if (isInclusive) {
            entry->attributes |= bit == 'r' ? READ_ONLY : WRITE_ONLY;
        } else {
            entry->attributes &= bit == 'r' ? ~READ_ONLY : ~WRITE_ONLY;
        }
    }

    entry->lastModifiedTime = static_cast<uint32_t>(time(nullptr));
}

void FileSystem::addPassword(const string &path, const string &password) {
    if (isRootDirectory(path)) {
        cerr << "addpw: '\\': Permission denied" << endl;
        return;
    }

    DirectoryEntry *entry = getDirectoryEntry(path);

    if (!entry) {
        cerr << "addpw: " << path << ": No such file or directory" << endl;
        return;
    }

    if (isDirectory(entry->attributes)) {
        cerr << "addpw: " << path << ": Not a file" << endl;
        return;
    }

    if (isPasswordProtected(entry->attributes)) {
        cerr << "addpw: " << path << ": Password already exists" << endl;
        return;
    }

    if (password.size() > MAX_PASSWORD) {
        cerr << "addpw: " << path << ": Password too long" << endl;
        return;
    }

    entry->attributes |= PASSWORD;
    entry->password = stou8Array<MAX_PASSWORD>(password);
    entry->lastModifiedTime = static_cast<uint32_t>(time(nullptr));
}

void FileSystem::removeDirectory(const string &path) {
    if (isRootDirectory(path)) {
        cerr << "rmdir: '\\': Permission denied" << endl;
        return;
    }

    DirectoryEntry *entry = getDirectoryEntry(path);

    if (!entry) {
        cerr << "rmdir: " << path << ": No such file or directory" << endl;
        return;
    }

    if (!isDirectory(entry->attributes)) {
        cerr << "rmdir: " << path << ": Not a directory" << endl;
        return;
    }

    if (entry->size > 2 * sizeof(DirectoryEntry)) {
        cerr << "rmdir: " << path << ": Directory not empty" << endl;
        return;
    }

    DirectoryEntry *parentEntry = getParentDirectoryEntry(path);
    int blockNum = parentEntry->firstBlock;
    int prevBlockNum = blockNum;

    if (isWritable(parentEntry->attributes) == 0) {
        cerr << "rmdir: " << path << ": Permission denied" << endl;
        return;
    }

    do {
        DirectoryEntry *dirPtr = reinterpret_cast<DirectoryEntry *>(fsBlocks.get() + blockNum * superBlock.blockSize);

        for (int i = 0; i < superBlock.maxEntryPerBlock; i++) {
            if (dirPtr[i].filename == entry->filename && dirPtr[i].extension == entry->extension) {
                dirPtr[i].filename = stou8Array<MAX_FILENAME>("");
                dirPtr[i].extension = stou8Array<MAX_EXTENSION>("");
                dirPtr[i].attributes = 0;
                dirPtr[i].creationTime = 0;
                dirPtr[i].lastModifiedTime = 0;
                dirPtr[i].size = 0;
                dirPtr[i].password = stou8Array<MAX_PASSWORD>("");
                freeRelatedBlocks(dirPtr[i].firstBlock);
                dirPtr[i].firstBlock = 0;
                parentEntry->size -= sizeof(DirectoryEntry);
                parentEntry->lastModifiedTime = static_cast<uint32_t>(time(nullptr));
                superBlock.directoryCount--;

                if (isBlockEmpty(blockNum)) {
                    fat[prevBlockNum] = fat[blockNum];
                    fat[blockNum] = FREE;
                    superBlock.freeBlocks++;
                }
                return;
            }
        }

        prevBlockNum = blockNum;
        blockNum = fat[blockNum];
    } while (blockNum != END_OF_FILE);
}

void FileSystem::freeBlock(const int16_t block) {
    uint8_t *blockPtr = fsBlocks.get() + block * superBlock.blockSize;

    for (int i = 0; i < superBlock.blockSize; i++) {
        blockPtr[i] = 0;
    }
}

void FileSystem::freeRelatedBlocks(const uint16_t firstBlock) {
    int blockNum = firstBlock;
    int nextBlock;

    do {
        nextBlock = fat[blockNum];
        freeBlock(blockNum);
        fat[blockNum] = FREE;
        superBlock.freeBlocks++;
        blockNum = nextBlock;
    } while (blockNum != END_OF_FILE);
}

void FileSystem::printFileSystemInfo() {
    vector<BlockInfo> blockMapping;

    blockMapping.push_back({"SuperBlock", 0});

    for (int16_t i = superBlock.fatFirstBlock; i < superBlock.rootDirectory.firstBlock; i++) {
        blockMapping.push_back({"FAT", i});
    }

    generateBlockMapping(blockMapping, superBlock.rootDirectory.firstBlock, getRootFilename(), true);
    sort(blockMapping.begin(), blockMapping.end(), [](const BlockInfo &a, const BlockInfo &b) { return a.blockNum < b.blockNum; });

    cout << string(5, '-') << "File System Information" << string(5, '-') << endl;
    cout << "Block Size: " << superBlock.blockSize << endl;
    cout << "Block Count: " << superBlock.blockCount << endl;
    cout << "Free Blocks: " << superBlock.freeBlocks << endl;
    cout << "File Count: " << superBlock.fileCount << endl;
    cout << "Directory Count: " << superBlock.directoryCount << endl;
    cout << endl << string(8, '-') << "Block Information" << string(8, '-') << endl;

    for (const auto &block : blockMapping) {
        cout << "Block #" << block.blockNum << ": " << block.filename << endl;
    }
}

void FileSystem::generateBlockMapping(vector<BlockInfo> &blockMapping, const int16_t firstBlock, const string &filename, const bool isDir) {
    int16_t blockNum = firstBlock;

    do {
        if (isDir) {
            DirectoryEntry *dirPtr = reinterpret_cast<DirectoryEntry *>(fsBlocks.get() + blockNum * superBlock.blockSize);

            for (int i = 0; i < superBlock.maxEntryPerBlock; i++) {
                if (dirPtr[i].filename[0] == 0 || dirPtr[i].filename == stou8Array<MAX_FILENAME>(".") ||
                    dirPtr[i].filename == stou8Array<MAX_FILENAME>("..")) {
                    continue;
                }

                string currentPath = filename.back() == getRootFilename()[0] ? filename : filename + getRootFilename();
                string combinedFilename = currentPath + fullFilename(dirPtr[i].filename, dirPtr[i].extension);
                generateBlockMapping(blockMapping, dirPtr[i].firstBlock, combinedFilename, isDirectory(dirPtr[i].attributes));
            }
        }

        blockMapping.push_back({filename, blockNum});
        blockNum = fat[blockNum];
    } while (blockNum != END_OF_FILE);
}

bool FileSystem::isBlockEmpty(const int16_t block) {
    uint8_t *blockPtr = fsBlocks.get() + block * superBlock.blockSize;

    for (int i = 0; i < superBlock.blockSize; i++) {
        if (blockPtr[i] != 0) {
            return false;
        }
    }

    return true;
}

void FileSystem::writeFile(const string &dest, const string &src, const string &password) {
    if (isRootDirectory(dest)) {
        cerr << "write: '" << getRootFilename() << "': Permission denied" << endl;
        return;
    }

    ifstream srcFile(src);

    if (!srcFile.is_open()) {
        cerr << "write: " << src << ": No such file or directory" << endl;
        return;
    }

    DirectoryEntry *entry = getDirectoryEntry(dest);

    if (entry && isDirectory(entry->attributes)) {
        cerr << "write: " << dest << ": Is a directory" << endl;
        return;
    }

    if (entry && !checkPassword(entry, password, dest, WRITE_FILE)) {
        return;
    }

    if (entry && !isWritable(entry->attributes)) {
        cerr << "write: " << dest << ": Permission denied" << endl;
        return;
    }

    DirectoryEntry *parentEntry = getParentDirectoryEntry(dest);

    if (!parentEntry) {
        cerr << "write: " << dest.substr(0, dest.find_last_of(getRootFilename())) << ": No such file or directory" << endl;
        return;
    }

    if (!isDirectory(parentEntry->attributes)) {
        cerr << "write: " << dest.substr(0, dest.find_last_of(getRootFilename())) << ": Not a directory" << endl;
        return;
    }

    if (!isWritable(parentEntry->attributes)) {
        cerr << "write: " << dest.substr(0, dest.find_last_of(getRootFilename())) << ": Permission denied" << endl;
        return;
    }

    if (entry) {
        freeRelatedBlocks(entry->firstBlock);
        fat[entry->firstBlock] = END_OF_FILE;
        entry->size = 0;
    } else {
        entry = addDirectoryEntry(dest, parentEntry, false);

        if (!entry) {
            return;
        }
    }

    if (!copyFile(srcFile, entry)) {
        if (fsError == NOT_ENOUGH_SPACE) {
            cerr << "write: " << dest << ": No space left" << endl;
        } else {
            cerr << "write: " << src << ": Failed to write file" << endl;
        }

        deleteFile(dest, password);
    }

    srcFile.close();
}

bool FileSystem::copyFile(ifstream &src, DirectoryEntry *dest) {
    int16_t blockNum = dest->firstBlock;
    int16_t prevBlockNum = blockNum;
    int16_t newBlockNum;
    uint8_t *blockPtr;
    uint32_t totalSize = 0;

    while (!src.eof()) {
        if (blockNum == END_OF_FILE) {
            newBlockNum = findFreeBlock();

            if (newBlockNum == -1) {
                fsError = NOT_ENOUGH_SPACE;
                return false;
            }

            fat[prevBlockNum] = newBlockNum;
            fat[newBlockNum] = END_OF_FILE;
            blockNum = newBlockNum;
            prevBlockNum = newBlockNum;
            superBlock.freeBlocks--;
        }

        blockPtr = fsBlocks.get() + blockNum * superBlock.blockSize;
        src.read(reinterpret_cast<char *>(blockPtr), superBlock.blockSize);

        if (src.fail() && !src.eof()) {
            fsError = FS_READ_FAILED;
            return false;
        }

        totalSize += src.gcount();
        blockNum = fat[blockNum];
    }

    dest->size = totalSize;
    dest->lastModifiedTime = static_cast<uint32_t>(time(nullptr));
    return true;
}

void FileSystem::deleteFile(const string &path, const string &password) {
    if (isRootDirectory(path)) {
        cerr << "del: '" << getRootFilename() << "': Permission denied" << endl;
        return;
    }

    DirectoryEntry *entry = getDirectoryEntry(path);

    if (!entry) {
        cerr << "del: " << path << ": No such file or directory" << endl;
        return;
    }

    if (isDirectory(entry->attributes)) {
        cerr << "del: " << path << ": Is a directory" << endl;
        return;
    }

    if (!checkPassword(entry, password, path, DELETE_FILE)) {
        return;
    }

    DirectoryEntry *parentEntry = getParentDirectoryEntry(path);

    if (isWritable(parentEntry->attributes) == 0) {
        cerr << "del: " << path << ": Permission denied" << endl;
        return;
    }

    int blockNum = parentEntry->firstBlock;
    int prevBlockNum = blockNum;

    do {
        DirectoryEntry *dirPtr = reinterpret_cast<DirectoryEntry *>(fsBlocks.get() + blockNum * superBlock.blockSize);

        for (int i = 0; i < superBlock.maxEntryPerBlock; i++) {
            if (dirPtr[i].filename == entry->filename && dirPtr[i].extension == entry->extension) {
                dirPtr[i].filename = stou8Array<MAX_FILENAME>("");
                dirPtr[i].extension = stou8Array<MAX_EXTENSION>("");
                dirPtr[i].attributes = 0;
                dirPtr[i].creationTime = 0;
                dirPtr[i].lastModifiedTime = 0;
                dirPtr[i].size = 0;
                dirPtr[i].password = stou8Array<MAX_PASSWORD>("");
                freeRelatedBlocks(dirPtr[i].firstBlock);
                dirPtr[i].firstBlock = 0;
                parentEntry->size -= sizeof(DirectoryEntry);
                parentEntry->lastModifiedTime = static_cast<uint32_t>(time(nullptr));
                superBlock.fileCount--;

                if (isBlockEmpty(blockNum)) {
                    fat[prevBlockNum] = fat[blockNum];
                    fat[blockNum] = FREE;
                    superBlock.freeBlocks++;
                }
                return;
            }
        }

        prevBlockNum = blockNum;
        blockNum = fat[blockNum];
    } while (blockNum != END_OF_FILE);
}

bool FileSystem::isRootDirectory(const string &path) {
    return path == getRootFilename() || path == getRootFilename() + "." || path == getRootFilename() + "..";
}

bool FileSystem::checkPassword(DirectoryEntry *entry, const string &password, const string &path, const FileSystemOperation &operation) {
    if (isPasswordProtected(entry->attributes)) {
        if (password.empty()) {
            cerr << operations[operation].name << ": " << path << ": Password required" << endl;
            return false;
        }

        string entryPassword = u8ArrayToString(entry->password);

        if (password != entryPassword) {
            cerr << operations[operation].name << ": " << path << ": Wrong password" << endl;
            return false;
        }
    }

    return true;
}

string FileSystem::getRootFilename() {
    return fullFilename(superBlock.rootDirectory.filename);
}

DirectoryEntry *FileSystem::getParentDirectoryEntry(const string &path) {
    string parentPath = path.substr(0, path.find_last_of(getRootFilename()));
    parentPath = parentPath.empty() ? getRootFilename() : parentPath;
    return getDirectoryEntry(parentPath);
}

pair<string, string> FileSystem::splitFilename(const string &filename) {
    size_t dotPosition = filename.find(".");
    dotPosition = dotPosition == string::npos ? filename.size() : dotPosition;
    return {filename.substr(0, dotPosition), filename.substr(dotPosition + (dotPosition == filename.size() ? 0 : 1))};
}

void FileSystem::readFile(const string &src, const string &dest, const string &password) {
    if (isRootDirectory(src)) {
        cerr << "read: '" << getRootFilename() << "': Permission denied" << endl;
        return;
    }

    DirectoryEntry *entry = getDirectoryEntry(src);

    if (!entry) {
        cerr << "read: " << src << ": No such file or directory" << endl;
        return;
    }

    if (isDirectory(entry->attributes)) {
        cerr << "read: " << src << ": Is a directory" << endl;
        return;
    }

    if (!checkPassword(entry, password, src, READ_FILE)) {
        return;
    }

    if (!isReadable(entry->attributes)) {
        cerr << "read: " << src << ": Permission denied" << endl;
        return;
    }

    ofstream destFile(dest);

    if (destFile.fail()) {
        cerr << "read: " << dest << ": Could not create file" << endl;
        return;
    }

    int16_t blockNum = entry->firstBlock;
    uint8_t *blockPtr;
    uint16_t lastBlockSize = entry->size % superBlock.blockSize;
    lastBlockSize = lastBlockSize == 0 ? superBlock.blockSize : lastBlockSize;

    while (blockNum != END_OF_FILE) {
        blockPtr = fsBlocks.get() + blockNum * superBlock.blockSize;

        if (fat[blockNum] == END_OF_FILE) {
            destFile.write(reinterpret_cast<char *>(blockPtr), lastBlockSize);
        } else {
            destFile.write(reinterpret_cast<char *>(blockPtr), superBlock.blockSize);
        }

        if (destFile.fail()) {
            fsError = FS_WRITE_FAILED;
            cerr << "read: " << dest << ": Failed to write file" << endl;
            break;
        }

        blockNum = fat[blockNum];
    }

    destFile.close();
}

DirectoryEntry *FileSystem::addDirectoryEntry(const string &path, DirectoryEntry *parentEntry, const bool isDir) {
    DirectoryEntry *dirPtr;
    int blockNum = parentEntry->firstBlock;
    int prevBlockNum = blockNum;
    pair<string, string> fullname = splitFilename(path.substr(path.find_last_of(getRootFilename()) + 1));

    if (fullname.first.size() > MAX_FILENAME || fullname.second.size() > MAX_EXTENSION) {
        cerr << (isDir ? "mkdir: " : "write: ") << path << ": File name too long" << endl;
        return nullptr;
    }

    if (superBlock.freeBlocks == 0) {
        cerr << (isDir ? "mkdir: " : "write: ") << path << ": No space left" << endl;
        return nullptr;
    }

    do {
        dirPtr = reinterpret_cast<DirectoryEntry *>(fsBlocks.get() + blockNum * superBlock.blockSize);

        for (int i = 0; i < superBlock.maxEntryPerBlock; i++) {
            if (dirPtr[i].filename[0] == 0) {
                initializeDirectoryEntry(&dirPtr[i], parentEntry, fullname, isDir);
                superBlock.freeBlocks--;
                return &dirPtr[i];
            }
        }

        prevBlockNum = blockNum;
        blockNum = fat[blockNum];
    } while (blockNum != END_OF_FILE);

    if (superBlock.freeBlocks < 2) {
        cerr << (isDir ? "mkdir: " : "write: ") << path << ": No space left" << endl;
        return nullptr;
    }

    int16_t newBlockNum = findFreeBlock();
    fat[prevBlockNum] = newBlockNum;
    fat[newBlockNum] = END_OF_FILE;

    dirPtr = reinterpret_cast<DirectoryEntry *>(fsBlocks.get() + newBlockNum * superBlock.blockSize);
    initializeDirectoryEntry(dirPtr, parentEntry, fullname, isDir);
    superBlock.freeBlocks -= 2;
    return dirPtr;
}

void FileSystem::initializeDirectoryEntry(DirectoryEntry *entry, DirectoryEntry *parentEntry, const pair<string, string> &fullname,
                                          const bool isDir) {
    entry->filename = stou8Array<MAX_FILENAME>(fullname.first);
    entry->extension = stou8Array<MAX_EXTENSION>(fullname.second);
    entry->attributes = READ_WRITE | (isDir ? DIRECTORY : 0);
    entry->firstBlock = findFreeBlock();
    entry->creationTime = static_cast<uint32_t>(time(nullptr));
    entry->lastModifiedTime = entry->creationTime;
    entry->size = isDir ? 2 * sizeof(DirectoryEntry) : 0;

    if (isDir) {
        DirectoryEntry *newDir = reinterpret_cast<DirectoryEntry *>(fsBlocks.get() + entry->firstBlock * superBlock.blockSize);

        for (int j = 0; j < 2; j++) {
            newDir[j].filename = stou8Array<MAX_FILENAME>(string(j + 1, '.'));
            newDir[j].extension = stou8Array<MAX_EXTENSION>("");
            newDir[j].attributes = entry->attributes;
            newDir[j].firstBlock = j == 0 ? entry->firstBlock : parentEntry->firstBlock;
        }
    }

    fat[entry->firstBlock] = END_OF_FILE;
    parentEntry->size += sizeof(DirectoryEntry);
    parentEntry->lastModifiedTime = entry->creationTime;

    if (isDir) {
        superBlock.directoryCount++;
    } else {
        superBlock.fileCount++;
    }
}
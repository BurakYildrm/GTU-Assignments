#include <iostream>

#include "fileSystem.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <blockSize> <filename>" << endl;
        return 1;
    }

    float blockSize = stof(argv[1]);
    string filename = argv[2];

    if (blockSize != 0.5 && blockSize != 1) {
        cout << "Error: Block size must be either 0.5 KB or 1 KB" << endl;
        return 1;
    }

    FileSystem fs(filename, static_cast<uint32_t>(blockSize * KB));
    fs.writeFileSystem();

    cout << "File system '" << filename << "' has been successfully created with a block size of " << blockSize << " KB." << endl;
    return 0;
}
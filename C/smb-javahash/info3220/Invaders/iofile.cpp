#include "iofile.h"

#include <iostream>

IOFile::IOFile(std::string filename)
{
    this->file.open(filename);

    if (file.is_open()) {
        // file could be opened
        std::string line;
        // put all lines in file to vector
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
    } else {
        // file doesn't exist, or couldn't be opened
        std::cout << "failed to open file: " << filename << std::endl;
        throw 1;
    }
}

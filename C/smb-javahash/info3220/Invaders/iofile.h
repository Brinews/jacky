#ifndef IOFILE_H
#define IOFILE_H

#include <fstream>
#include <string>
#include <vector>

/**
 * @brief The IOFile class
 * Interprets files as a list of lines
 */
class IOFile
{
    std::fstream file; // the file on hard drive
    std::vector<std::string> lines; // will have all lines in the file

public:
    IOFile(std::string filename);
    virtual ~IOFile() {}

    const std::vector<std::string> getLines() const { return lines; }
};

#endif // IOFILE_H

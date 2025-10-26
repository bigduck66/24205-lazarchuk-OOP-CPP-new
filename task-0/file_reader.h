#ifndef FILE_READER_H
#define FILE_READER_H

#include <fstream>
#include <iostream>
#include <list>
#include <string>

class FileReader {
private:
    std::list<std::string> lines;

public:
    FileReader();
    bool readFile(const std::string& filename);
    const std::list<std::string>& getLines() const;
    size_t getLineCount() const;
    void clear();
};

#endif
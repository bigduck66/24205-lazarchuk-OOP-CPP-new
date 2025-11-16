#ifndef FILE_READER_H
#define FILE_READER_H

#include <fstream>
#include <string>

class FileReader {
private:
    std::string fileName;
    std::ifstream in;
    std::string line;
public:
    FileReader() = default;
    explicit FileReader(const std::string& fileName);
    ~FileReader();
    bool isOpen() const;
    bool isEOF() const;
    const std::string& getLine();
    void ronka();
    void ronka_ushel();
};

#endif
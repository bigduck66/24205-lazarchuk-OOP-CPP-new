#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include <string>
#include <vector>
#include <fstream>

class CSVWriter {
private:
    std::string fileName;
    std::ofstream out;

public:
    explicit CSVWriter(const std::string& fileName);
    ~CSVWriter();
    bool isOpen() const;
    void write(const std::vector<std::string>& values);
    void open(const std::string& fileName);
    void close();
};

#endif
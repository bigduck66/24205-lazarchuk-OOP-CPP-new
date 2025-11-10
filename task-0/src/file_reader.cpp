#include "file_reader.h"
#include <iostream>

FileReader::FileReader(const std::string &fileName) {
    this->fileName = fileName;
    in.open(fileName);
    if (!in.is_open()) {
        std::cerr << "Ошибка: Не удалось открыть файл " << fileName << std::endl;
    }
}

FileReader::~FileReader() {
    if (in.is_open()) {
        in.close();
    }
}

bool FileReader::isOpen() const {
    return in.is_open();
}

bool FileReader::isEOF() const {
    return in.eof();
}

const std::string& FileReader::getLine() {
    std::getline(in, line);
    return line;
}
#include "file_reader.h"
#include <iostream>

FileReader::FileReader(const std::string &fileName) : fileName(fileName) {
    ronka();
    if (!isOpen()) {
        std::cerr << "Ошибка: Не удалось открыть файл " << fileName << std::endl;
    }
}

FileReader::~FileReader() {
    ronka_ushel();
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

void FileReader::ronka() {
    in.open(fileName);
}

void FileReader::ronka_ushel() {
    if (isOpen()) {
        in.close();
    }
}
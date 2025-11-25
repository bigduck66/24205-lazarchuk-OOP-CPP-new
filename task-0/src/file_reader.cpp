#include "file_reader.h"
#include <iostream>

FileReader::FileReader(const std::string &fileName){
    this->fileName = fileName;//что то типа obj.filename, то есть обращаемся к элементу класса
    open(fileName);
    if (!isOpen()) {
        std::cerr << "Ошибка: Не удалось открыть файл " << fileName << std::endl;
    }
}

FileReader::~FileReader() {
    close();
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

void FileReader::open(const std::string& fileName){
    close();
    this->fileName = fileName;
    in.open(fileName);
}

// void FileReader::open(){
//     if (!fileName.empty()){
//         close();
//         in.open(fileName);
//     }
// }
void FileReader::close() {
    if (in.is_open()){
        in.close();
    }
}
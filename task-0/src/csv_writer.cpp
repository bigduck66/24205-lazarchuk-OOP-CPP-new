#include "csv_writer.h"
#include <iostream>

CSVWriter::CSVWriter(const std::string &fileName) {
    this->fileName = fileName;
    out.open(fileName);
    if (!out.is_open()) {
        std::cerr << "Ошибка: Не удалось создать файл " << fileName << std::endl;
    }
}

CSVWriter::~CSVWriter() {
    if (out.is_open()) {
        out.close();
    }
}

bool CSVWriter::isOpen() const {
    return out.is_open();
}

void CSVWriter::write(const std::vector<std::string>& values) {
    if (!out.is_open()) {
        std::cerr << "Ошибка: Файл не открыт для записи" << std::endl;
        return;
    }

    for (size_t i = 0; i < values.size(); ++i) {
        out << values[i];
        if (i != values.size() - 1) {
            out << ",";
        }
    }
    out << "\n";
    out.flush();
}
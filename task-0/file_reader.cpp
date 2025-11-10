#include "file_reader.h"

FileReader::FileReader() {}

bool FileReader::readFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Ошибка: Не удалось открыть файл " << filename << std::endl;
        return false;
    }

    lines.clear();
    std::string line;
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }

    inputFile.close();
    std::cout << "Прочитано строк: " << lines.size() << std::endl;
    return true;
}

const std::list<std::string>& FileReader::getLines() const {// Возвращает константную ссылку на список строк 
    return lines;
}

size_t FileReader::getLineCount() const {// Возвращает количество строк в файле
    return lines.size();
}

void FileReader::clear() {// Очищает список строк
    lines.clear();
}
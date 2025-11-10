#include "csv_writer.h"
#include <iostream>

CSVWriter::CSVWriter() {}

bool CSVWriter::writeCSV(const std::string& filename, 
                        const std::vector<std::pair<std::string, int>>& sortedWords,
                        int totalWords) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Ошибка: Не удалось создать файл " << filename << std::endl;
        return false;
    }
    outputFile << "Слово,Частота,Частота (%)\n";
    for (size_t i = 0; i < sortedWords.size(); ++i) {
        const std::string& word = sortedWords[i].first;
        int count = sortedWords[i].second;
        double percentage = (static_cast<double>(count) / totalWords) * 100.0;//static_cast чтобы делить вещественно
        outputFile << word << "," << count << "," 
                   << std::fixed << std::setprecision(4) << percentage << "\n";//4 знаков после запятой
    }
    outputFile.close();
    std::cout << "Результат записан в файл: " << filename << std::endl;
    return true;
}
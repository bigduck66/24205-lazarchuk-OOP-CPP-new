#include <iostream>
#include <iomanip>
#include <sstream>
#include "file_reader.h"
#include "text_processor.h"
#include "word_analyzer.h"
#include "csv_writer.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Использование: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    // Открываем файл для чтения
    FileReader reader(argv[1]);
    if (!reader.isOpen()) {
        std::cerr << "Ошибка: Не удалось открыть входной файл " << argv[1] << std::endl;
        return 1;
    }

    // Создаем процессор и анализатор
    TextProcessor processor;
    WordAnalyzer analyzer;

    // Обрабатываем файл построчно
    while (!reader.isEOF()) {
        const std::string& line = reader.getLine();
        const std::vector<std::string> words = processor.processLine(line);
        analyzer.addWords(words);
    }

    // Открываем файл для записи
    CSVWriter writer(argv[2]);
    if (!writer.isOpen()) {
        std::cerr << "Ошибка: Не удалось создать выходной файл " << argv[2] << std::endl;
        return 1;
    }

    // Записываем заголовок
    writer.write({"Слово", "Частота", "Частота (%)"});

    // Записываем статистику
    std::vector<std::pair<std::string, int>> sortedWords = analyzer.getSortedWords();
    int totalWords = analyzer.getTotalWords();

    for (const auto& wordStat : sortedWords) {
        const std::string& word = wordStat.first;
        int count = wordStat.second;
        double percentage = (static_cast<double>(count) / totalWords) * 100.0;
        
        std::ostringstream freqStream;
        freqStream << std::fixed << std::setprecision(4) << percentage;
        
        std::vector<std::string> row = {
            word,
            std::to_string(count),
            freqStream.str()
        };
        writer.write(row);
    }

    std::cout << "Программа завершена успешно!" << std::endl;
    std::cout << "Обработано слов: " << analyzer.getTotalWords() << std::endl;
    std::cout << "Уникальных слов: " << analyzer.getUniqueWordsCount() << std::endl;
    std::cout << "Результат записан в: " << argv[2] << std::endl;

    return 0;
}
#include <iostream>
#include "file_reader.h"
#include "text_processor.h"
#include "word_analyzer.h"
#include "csv_writer.h"

// Функция для запуска обычного режима
int runNormalMode(const std::string& inputFile, const std::string& outputFile) {
    // Создаем объекты классов
    FileReader fileReader;
    WordAnalyzer wordAnalyzer;

    // Читаем файл
    if (!fileReader.readFile(inputFile)) {
        return 1;
    }

    // TextProcessor полностью подготавливает данные
    std::vector<std::string> processedWords = 
        TextProcessor::processLines(fileReader.getLines());

    // WordAnalyzer только анализирует готовые слова
    wordAnalyzer.analyzeWords(processedWords);

    // Записываем результат
    if (!CSVWriter::writeCSV(outputFile, 
                           wordAnalyzer.getSortedWords(),
                           wordAnalyzer.getTotalWords())) {
        return 1;
    }

    std::cout << "Программа завершена успешно!" << std::endl;
    std::cout << "Итоговая статистика:" << std::endl;
    std::cout << "- Всего слов: " << wordAnalyzer.getTotalWords() << std::endl;
    std::cout << "- Уникальных слов: " << wordAnalyzer.getUniqueWordsCount() << std::endl;

    return 0;
}

// Функция для запуска тестов
int runTests() {
    std::cout << "Для запуска тестов скомпилируйте и запустите test_word_counter:" << std::endl;
    std::cout << "g++ -std=c++17 -I. test_word_counter.cpp file_reader.cpp text_processor.cpp word_analyzer.cpp csv_writer.cpp -lgtest -lgtest_main -lpthread -o test_word_counter" << std::endl;
    std::cout << "./test_word_counter" << std::endl;
    return 0;
}

int main(int argc, char* argv[]) {
    // Если передан аргумент --test, запускаем тесты
    if (argc == 2 && std::string(argv[1]) == "--test") {
        return runTests();
    }
    
    // Обычный режим работы
    if (argc != 3) {
        std::cerr << "Использование: " << argv[0] << " input.txt output.csv" << std::endl;
        std::cerr << "Или для тестов: " << argv[0] << " --test" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    return runNormalMode(inputFile, outputFile);
}
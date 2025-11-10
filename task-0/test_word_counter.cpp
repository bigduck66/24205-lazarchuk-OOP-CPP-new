#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include "file_reader.h"
#include "text_processor.h"
#include "word_analyzer.h"
#include "csv_writer.h"
#include <iomanip>

class WordCounterTest : public ::testing::Test {
protected:
    void SetUp() override {
        createTestFiles();
    }

    void TearDown() override {
        removeTestFiles();
    }

    void createTestFiles() {
        // Тестовый файл 1: простой текст
        std::ofstream file1("test_simple.txt");
        file1 << "Hello world! Hello everyone. World is beautiful.";
        file1.close();

        // Тестовый файл 2: текст с разными разделителями
        std::ofstream file2("test_delimiters.txt");
        file2 << "word1, word2; word3. word4? word5! word1 word2";
        file2.close();

        // Тестовый файл 3: пустой файл
        std::ofstream file3("test_empty.txt");
        file3.close();

        // Тестовый файл 4: текст с числами
        std::ofstream file4("test_numbers.txt");
        file4 << "test123 456test test 123 test";
        file4.close();

        // Тестовый файл 5: русский текст
        std::ofstream file5("test_russian.txt");
        file5 << "Привет мир! Привет всем. Мир прекрасен.";
        file5.close();
    }

    void removeTestFiles() {
        std::remove("test_simple.txt");
        std::remove("test_delimiters.txt");
        std::remove("test_empty.txt");
        std::remove("test_numbers.txt");
        std::remove("test_russian.txt");
        std::remove("test_output.csv");
    }

    std::string readFileContent(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return "";
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
};

// Тесты для FileReader
TEST_F(WordCounterTest, FileReader_ReadFile) {
    FileReader reader("test_simple.txt");
    EXPECT_TRUE(reader.isOpen());
    
    // Читаем все строки
    while (!reader.isEOF()) {
        const std::string& line = reader.getLine();
        EXPECT_FALSE(line.empty());
    }
}

TEST_F(WordCounterTest, FileReader_ReadNonExistentFile) {
    FileReader reader("nonexistent_file.txt");
    EXPECT_FALSE(reader.isOpen());
}

TEST_F(WordCounterTest, FileReader_EmptyFile) {
    FileReader reader("test_empty.txt");
    EXPECT_TRUE(reader.isOpen());
    EXPECT_TRUE(reader.isEOF()); // Пустой файл сразу в EOF
}

// Тесты для TextProcessor
TEST_F(WordCounterTest, TextProcessor_ParseLine) {
    TextProcessor processor;
    std::string testLine = "word1, word2; word3.word4?word5!";
    auto words = processor.processLine(testLine);
    
    EXPECT_EQ(words.size(), 5);
    EXPECT_EQ(words[0], "word1");
    EXPECT_EQ(words[1], "word2");
    EXPECT_EQ(words[2], "word3");
    EXPECT_EQ(words[3], "word4");
    EXPECT_EQ(words[4], "word5");
    
    // Проверяем что все в нижнем регистре
    for (const auto& word : words) {
        for (char c : word) {
            EXPECT_TRUE(std::islower(static_cast<unsigned char>(c)) || 
                       std::isdigit(static_cast<unsigned char>(c)));
        }
    }
}

TEST_F(WordCounterTest, TextProcessor_EmptyLine) {
    TextProcessor processor;
    std::string emptyLine = "";
    auto words = processor.processLine(emptyLine);
    EXPECT_TRUE(words.empty());
}

TEST_F(WordCounterTest, TextProcessor_OnlyDelimiters) {
    TextProcessor processor;
    std::string line = "!@#$%^&*()";
    auto words = processor.processLine(line);
    EXPECT_TRUE(words.empty());
}

// Тесты для WordAnalyzer
TEST_F(WordCounterTest, WordAnalyzer_BasicAnalysis) {
    WordAnalyzer analyzer;
    std::vector<std::string> words = {"hello", "world", "hello", "test"};
    
    analyzer.addWords(words);
    
    EXPECT_EQ(analyzer.getTotalWords(), 4);
    EXPECT_EQ(analyzer.getUniqueWordsCount(), 3);
    
    auto sorted = analyzer.getSortedWords();
    EXPECT_EQ(sorted[0].first, "hello");
    EXPECT_EQ(sorted[0].second, 2);
    EXPECT_EQ(sorted[1].first, "world");
    EXPECT_EQ(sorted[1].second, 1);
    EXPECT_EQ(sorted[2].first, "test");
    EXPECT_EQ(sorted[2].second, 1);
}

TEST_F(WordCounterTest, WordAnalyzer_EmptyAnalysis) {
    WordAnalyzer analyzer;
    std::vector<std::string> words;
    
    analyzer.addWords(words);
    
    EXPECT_EQ(analyzer.getTotalWords(), 0);
    EXPECT_EQ(analyzer.getUniqueWordsCount(), 0);
    EXPECT_TRUE(analyzer.getSortedWords().empty());
}

TEST_F(WordCounterTest, WordAnalyzer_Sorting) {
    WordAnalyzer analyzer;
    std::vector<std::string> words = {"a", "b", "c", "a", "b", "a"}; // a:3, b:2, c:1
    
    analyzer.addWords(words);
    auto sorted = analyzer.getSortedWords();
    
    EXPECT_EQ(sorted.size(), 3);
    EXPECT_EQ(sorted[0].first, "a");
    EXPECT_EQ(sorted[0].second, 3);
    EXPECT_EQ(sorted[1].first, "b");
    EXPECT_EQ(sorted[1].second, 2);
    EXPECT_EQ(sorted[2].first, "c");
    EXPECT_EQ(sorted[2].second, 1);
}

TEST_F(WordCounterTest, WordAnalyzer_Clear) {
    WordAnalyzer analyzer;
    std::vector<std::string> words = {"hello", "world"};
    
    analyzer.addWords(words);
    EXPECT_EQ(analyzer.getTotalWords(), 2);
    
    analyzer.clear();
    EXPECT_EQ(analyzer.getTotalWords(), 0);
    EXPECT_EQ(analyzer.getUniqueWordsCount(), 0);
}

TEST_F(WordCounterTest, WordAnalyzer_AddWord) {
    WordAnalyzer analyzer;
    analyzer.addWord("test");
    analyzer.addWord("test");
    analyzer.addWord("hello");
    
    EXPECT_EQ(analyzer.getTotalWords(), 3);
    EXPECT_EQ(analyzer.getUniqueWordsCount(), 2);
    
    auto sorted = analyzer.getSortedWords();
    EXPECT_EQ(sorted[0].first, "test");
    EXPECT_EQ(sorted[0].second, 2);
    EXPECT_EQ(sorted[1].first, "hello");
    EXPECT_EQ(sorted[1].second, 1);
}

// Тесты для CSVWriter
TEST_F(WordCounterTest, CSVWriter_BasicWrite) {
    CSVWriter writer("test_output.csv");
    EXPECT_TRUE(writer.isOpen());
    
    writer.write({"Слово", "Частота", "Частота (%)"});
    writer.write({"hello", "2", "50.0000"});
    writer.write({"world", "2", "50.0000"});
    
    // Деструктор закроет файл
}

TEST_F(WordCounterTest, CSVWriter_ContentCheck) {
    {
        CSVWriter writer("test_output.csv");
        writer.write({"word", "count", "percent"});
        writer.write({"test", "2", "100.0000"});
    } // Файл закрывается здесь
    
    std::string content = readFileContent("test_output.csv");
    EXPECT_FALSE(content.empty());
    EXPECT_NE(content.find("word,count,percent"), std::string::npos);
    EXPECT_NE(content.find("test,2,100.0000"), std::string::npos);
}

TEST_F(WordCounterTest, CSVWriter_MultipleValues) {
    {
        CSVWriter writer("test_output.csv");
        std::vector<std::string> row1 = {"header1", "header2", "header3"};
        std::vector<std::string> row2 = {"value1", "value2", "value3"};
        writer.write(row1);
        writer.write(row2);
    }
    
    std::string content = readFileContent("test_output.csv");
    EXPECT_NE(content.find("header1,header2,header3"), std::string::npos);
    EXPECT_NE(content.find("value1,value2,value3"), std::string::npos);
}

// Интеграционные тесты
TEST_F(WordCounterTest, Integration_FullPipeline) {
    // Чтение файла
    FileReader reader("test_simple.txt");
    EXPECT_TRUE(reader.isOpen());
    
    // Обработка текста
    TextProcessor processor;
    WordAnalyzer analyzer;
    
    while (!reader.isEOF()) {
        const std::string& line = reader.getLine();
        auto words = processor.processLine(line);
        analyzer.addWords(words);
    }
    
    EXPECT_GT(analyzer.getTotalWords(), 0);
    EXPECT_GT(analyzer.getUniqueWordsCount(), 0);
    
    // Запись в CSV
    CSVWriter writer("test_output.csv");
    EXPECT_TRUE(writer.isOpen());
    
    writer.write({"Слово", "Частота", "Частота (%)"});
    auto sortedWords = analyzer.getSortedWords();
    int totalWords = analyzer.getTotalWords();
    
    for (const auto& wordStat : sortedWords) {
        double percentage = (static_cast<double>(wordStat.second) / totalWords) * 100.0;
        std::ostringstream freqStream;
        freqStream << std::fixed << std::setprecision(4) << percentage;
        
        std::vector<std::string> row = {
            wordStat.first,
            std::to_string(wordStat.second),
            freqStream.str()
        };
        writer.write(row);
    }
    
    // Проверяем что файл создан и не пустой
    std::string content = readFileContent("test_output.csv");
    EXPECT_FALSE(content.empty());
    EXPECT_NE(content.find("Слово,Частота,Частота (%)"), std::string::npos);
}

TEST_F(WordCounterTest, Integration_NumbersInText) {
    FileReader reader("test_numbers.txt");
    TextProcessor processor;
    WordAnalyzer analyzer;
    
    while (!reader.isEOF()) {
        const std::string& line = reader.getLine();
        auto words = processor.processLine(line);
        analyzer.addWords(words);
    }
    
    auto sorted = analyzer.getSortedWords();
    
    // Ищем нужные слова в отсортированном списке
    bool foundTest = false, foundTest123 = false, found456test = false, found123 = false;
    
    for (const auto& wordStat : sorted) {
        if (wordStat.first == "test") foundTest = true;
        if (wordStat.first == "test123") foundTest123 = true;
        if (wordStat.first == "456test") found456test = true;
        if (wordStat.first == "123") found123 = true;
    }
    
    EXPECT_TRUE(foundTest);
    EXPECT_TRUE(foundTest123);
    EXPECT_TRUE(found456test);
    EXPECT_TRUE(found123);
}

TEST_F(WordCounterTest, Integration_RussianText) {
    FileReader reader("test_russian.txt");
    TextProcessor processor;
    WordAnalyzer analyzer;
    
    while (!reader.isEOF()) {
        const std::string& line = reader.getLine();
        auto words = processor.processLine(line);
        analyzer.addWords(words);
    }
    
    EXPECT_GT(analyzer.getTotalWords(), 0);
    EXPECT_GT(analyzer.getUniqueWordsCount(), 0);
    
    auto sorted = analyzer.getSortedWords();
    
    // Проверяем что русские слова обработаны (в нижнем регистре)
    for (const auto& wordStat : sorted) {
        for (char c : wordStat.first) {
            // Русские буквы или строчные английские/цифры
            EXPECT_TRUE(std::islower(static_cast<unsigned char>(c)) || 
                       std::isdigit(static_cast<unsigned char>(c)) ||
                       static_cast<unsigned char>(c) > 127); // Русские символы
        }
    }
}
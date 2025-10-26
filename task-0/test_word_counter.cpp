#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include "file_reader.h"
#include "text_processor.h"
#include "word_analyzer.h"
#include "csv_writer.h"

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
    FileReader reader;
    EXPECT_TRUE(reader.readFile("test_simple.txt"));
    EXPECT_GT(reader.getLineCount(), 0);
}

TEST_F(WordCounterTest, FileReader_ReadNonExistentFile) {
    FileReader reader;
    EXPECT_FALSE(reader.readFile("nonexistent_file.txt"));
}

TEST_F(WordCounterTest, FileReader_EmptyFile) {
    FileReader reader;
    EXPECT_TRUE(reader.readFile("test_empty.txt"));
    EXPECT_EQ(reader.getLineCount(), 0);
}

// Тесты для TextProcessor
TEST_F(WordCounterTest, TextProcessor_SplitIntoWords) {
    std::string testLine = "word1, word2; word3.word4?word5!";
    auto words = TextProcessor::splitIntoWords(testLine);
    
    EXPECT_EQ(words.size(), 5);
    EXPECT_EQ(words[0], "word1");
    EXPECT_EQ(words[1], "word2");
    EXPECT_EQ(words[2], "word3");
    EXPECT_EQ(words[3], "word4");
    EXPECT_EQ(words[4], "word5");
}

TEST_F(WordCounterTest, TextProcessor_ToLower) {
    EXPECT_EQ(TextProcessor::toLower("Hello"), "hello");
    EXPECT_EQ(TextProcessor::toLower("WORLD"), "world");
    EXPECT_EQ(TextProcessor::toLower("MixED"), "mixed");
}

TEST_F(WordCounterTest, TextProcessor_IsWordChar) {
    EXPECT_TRUE(TextProcessor::isWordChar('a'));
    EXPECT_TRUE(TextProcessor::isWordChar('Z'));
    EXPECT_TRUE(TextProcessor::isWordChar('5'));
    EXPECT_FALSE(TextProcessor::isWordChar(','));
    EXPECT_FALSE(TextProcessor::isWordChar(' '));
    EXPECT_FALSE(TextProcessor::isWordChar('!'));
}

TEST_F(WordCounterTest, TextProcessor_ProcessLines) {
    std::list<std::string> lines = {"Hello World!", "Test Line."};
    auto words = TextProcessor::processLines(lines);
    
    EXPECT_GT(words.size(), 0);
    for (const auto& word : words) {
        for (char c : word) {
            EXPECT_TRUE(std::isalnum(static_cast<unsigned char>(c)));
        }
    }
}

// Тесты для WordAnalyzer
TEST_F(WordCounterTest, WordAnalyzer_BasicAnalysis) {
    WordAnalyzer analyzer;
    std::vector<std::string> words = {"hello", "world", "hello", "test"};
    
    analyzer.analyzeWords(words);
    
    EXPECT_EQ(analyzer.getTotalWords(), 4);
    EXPECT_EQ(analyzer.getUniqueWordsCount(), 3);
    
    const auto& freq = analyzer.getWordFrequency();
    EXPECT_EQ(freq.at("hello"), 2);
    EXPECT_EQ(freq.at("world"), 1);
    EXPECT_EQ(freq.at("test"), 1);
}

TEST_F(WordCounterTest, WordAnalyzer_EmptyAnalysis) {
    WordAnalyzer analyzer;
    std::vector<std::string> words;
    
    analyzer.analyzeWords(words);
    
    EXPECT_EQ(analyzer.getTotalWords(), 0);
    EXPECT_EQ(analyzer.getUniqueWordsCount(), 0);
}

TEST_F(WordCounterTest, WordAnalyzer_Sorting) {
    WordAnalyzer analyzer;
    std::vector<std::string> words = {"a", "b", "c", "a", "b", "a"}; // a:3, b:2, c:1
    
    analyzer.analyzeWords(words);
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
    
    analyzer.analyzeWords(words);
    EXPECT_EQ(analyzer.getTotalWords(), 2);
    
    analyzer.clear();
    EXPECT_EQ(analyzer.getTotalWords(), 0);
    EXPECT_EQ(analyzer.getUniqueWordsCount(), 0);
}

// Тесты для CSVWriter
TEST_F(WordCounterTest, CSVWriter_BasicWrite) {
    std::vector<std::pair<std::string, int>> words = {
        {"hello", 2},
        {"world", 1}
    };
    
    EXPECT_TRUE(CSVWriter::writeCSV("test_output.csv", words, 3));
    
    std::string content = readFileContent("test_output.csv");
    EXPECT_FALSE(content.empty());
    EXPECT_NE(content.find("Слово,Частота,Частота (%)"), std::string::npos);
}

TEST_F(WordCounterTest, CSVWriter_ContentCheck) {
    std::vector<std::pair<std::string, int>> words = {
        {"test", 2}
    };
    
    EXPECT_TRUE(CSVWriter::writeCSV("test_output.csv", words, 2));
    
    std::string content = readFileContent("test_output.csv");
    EXPECT_NE(content.find("test,2,100.0000"), std::string::npos);
}

TEST_F(WordCounterTest, CSVWriter_PercentageCalculation) {
    std::vector<std::pair<std::string, int>> words = {
        {"word1", 1},
        {"word2", 2}
    };
    
    EXPECT_TRUE(CSVWriter::writeCSV("test_output.csv", words, 3));
    
    std::string content = readFileContent("test_output.csv");
    EXPECT_NE(content.find("33.3333"), std::string::npos); // 1/3 * 100
    EXPECT_NE(content.find("66.6667"), std::string::npos); // 2/3 * 100
}

// Интеграционные тесты
TEST_F(WordCounterTest, Integration_FullPipeline) {
    FileReader reader;
    WordAnalyzer analyzer;
    
    EXPECT_TRUE(reader.readFile("test_simple.txt"));
    
    auto processedWords = TextProcessor::processLines(reader.getLines());
    analyzer.analyzeWords(processedWords);
    
    EXPECT_GT(analyzer.getTotalWords(), 0);
    EXPECT_GT(analyzer.getUniqueWordsCount(), 0);
    
    auto sortedWords = analyzer.getSortedWords();
    EXPECT_TRUE(CSVWriter::writeCSV("test_output.csv", sortedWords, analyzer.getTotalWords()));
    
    // Проверяем что файл создан и не пустой
    std::string content = readFileContent("test_output.csv");
    EXPECT_FALSE(content.empty());
    EXPECT_NE(content.find("Слово,Частота,Частота (%)"), std::string::npos);
}

TEST_F(WordCounterTest, Integration_NumbersInText) {
    FileReader reader;
    WordAnalyzer analyzer;
    
    EXPECT_TRUE(reader.readFile("test_numbers.txt"));
    
    auto processedWords = TextProcessor::processLines(reader.getLines());
    analyzer.analyzeWords(processedWords);
    
    const auto& freq = analyzer.getWordFrequency();
    EXPECT_EQ(freq.at("test"), 2);
    EXPECT_EQ(freq.at("test123"), 1);
    EXPECT_EQ(freq.at("456test"), 1);
    EXPECT_EQ(freq.at("123"), 1);
}
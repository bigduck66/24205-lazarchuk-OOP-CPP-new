#include "word_analyzer.h"
#include <iostream>

WordAnalyzer::WordAnalyzer() : totalWords(0) {}

void WordAnalyzer::analyzeWords(const std::vector<std::string>& words) {// считает частоту каждого слова
    wordFrequency.clear();
    totalWords = 0;

    for (size_t i = 0; i < words.size(); ++i) { // Проходим по всем словам и увеличиваем частоту соответствующего слова
        const std::string& word = words[i];
        wordFrequency[word]++;
        totalWords++;
    }

    std::cout << "Обработано слов: " << totalWords << std::endl;
    std::cout << "Уникальных слов: " << wordFrequency.size() << std::endl;
}

struct CompareByFrequency {// Сортировка слов по частоте
    bool operator()(const std::pair<std::string, int>& a, 
                   const std::pair<std::string, int>& b) const {
        return a.second > b.second;
    }
};

std::vector<std::pair<std::string, int>> WordAnalyzer::getSortedWords() const {// Возвращает список слов в порядке убывания частоты
    std::vector<std::pair<std::string, int>> sortedWords(
        wordFrequency.begin(), wordFrequency.end()
    );
    std::sort(sortedWords.begin(), sortedWords.end(), CompareByFrequency());
    return sortedWords;
}

void WordAnalyzer::clear() {// Очищает словарь частот
    wordFrequency.clear();
    totalWords = 0;
}

int WordAnalyzer::getTotalWords() const {// Возвращает общее число слов
    return totalWords;
}

int WordAnalyzer::getUniqueWordsCount() const {// Возвращает количество уникальных слов
    return wordFrequency.size();
}

const std::map<std::string, int>& WordAnalyzer::getWordFrequency() const {// Возвращает константную ссылку на словарь частот
    return wordFrequency;
}
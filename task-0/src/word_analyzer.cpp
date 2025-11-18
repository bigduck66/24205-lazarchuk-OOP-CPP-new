#include "word_analyzer.h"
#include <algorithm>
#include <iostream>

WordAnalyzer::WordAnalyzer() : totalWords(0) {}

void WordAnalyzer::addWord(const std::string& word) {
    if (!word.empty()) {
        wordFrequency[word]++;
        totalWords++;
    }
}

void WordAnalyzer::addWords(const std::vector<std::string>& words) {
    for (const auto& word : words) {
        addWord(word);
    }
}

struct CompareByFrequency {
    bool operator()(const std::pair<std::string, int>& a, 
                   const std::pair<std::string, int>& b) const {
        return a.second > b.second;
    }
};

std::vector<std::pair<std::string, int>> WordAnalyzer::getSortedWords() const {
    std::vector<std::pair<std::string, int>> sortedWords(
        wordFrequency.begin(), wordFrequency.end()
    );
    std::sort(sortedWords.begin(), sortedWords.end(), CompareByFrequency());
    return sortedWords;
}

int WordAnalyzer::getTotalWords() const {
    return totalWords;
}

int WordAnalyzer::getUniqueWordsCount() const {
    return wordFrequency.size();
}

void WordAnalyzer::clear() {
    wordFrequency.clear();
    totalWords = 0;
}
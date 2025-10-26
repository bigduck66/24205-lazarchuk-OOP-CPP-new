#ifndef WORD_ANALYZER_H
#define WORD_ANALYZER_H

#include <string>
#include <map>
#include <vector>
#include <algorithm>

class WordAnalyzer {
private:
    std::map<std::string, int> wordFrequency;
    int totalWords;

public:
    WordAnalyzer();
    void analyzeWords(const std::vector<std::string>& words);
    void clear();
    int getTotalWords() const;
    int getUniqueWordsCount() const;
    std::vector<std::pair<std::string, int>> getSortedWords() const;
    const std::map<std::string, int>& getWordFrequency() const;
};

#endif
#ifndef WORD_ANALYZER_H
#define WORD_ANALYZER_H

#include <string>
#include <map>
#include <vector>
#include <utility>

class WordAnalyzer {
private:
    std::map<std::string, int> wordFrequency;
    int totalWords;

public:
    WordAnalyzer();
    void addWord(const std::string& word);
    void addWords(const std::vector<std::string>& words);
    std::vector<std::pair<std::string, int>> getSortedWords() const;
    int getTotalWords() const;
    int getUniqueWordsCount() const;
    void clear();
};

#endif
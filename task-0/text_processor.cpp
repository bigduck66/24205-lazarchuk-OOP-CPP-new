#include "text_processor.h"

TextProcessor::TextProcessor() {}

std::vector<std::string> TextProcessor::processLines(const std::list<std::string>& lines) {
    std::vector<std::string> allWords;
    
    for (std::list<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it) {
        std::vector<std::string> words = splitIntoWords(*it);
        for (size_t i = 0; i < words.size(); ++i) {
            if (!words[i].empty()) {
                allWords.push_back(toLower(words[i]));
            }
        }
    }
    
    return allWords;
}

std::string TextProcessor::toLower(const std::string& str) {
    std::string result = str;
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = std::tolower(static_cast<unsigned char>(result[i]));
    }
    return result;
}

std::vector<std::string> TextProcessor::splitIntoWords(const std::string& line) {
    std::vector<std::string> words;
    std::string currentWord;

    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];
        if (isWordChar(c)) {
            currentWord += c;
        } else {
            if (!currentWord.empty()) {
                words.push_back(currentWord);
                currentWord.clear();
            }
        }
    }

    if (!currentWord.empty()) {
        words.push_back(currentWord);
    }

    return words;
}

bool TextProcessor::isWordChar(char c) {
    return std::isalnum(static_cast<unsigned char>(c));
}
#ifndef TEXT_PROCESSOR_H
#define TEXT_PROCESSOR_H

#include <string>
#include <vector>
#include <list>
#include <cctype>

class TextProcessor {
public:
    TextProcessor();
    static std::vector<std::string> processLines(const std::list<std::string>& lines);
    static std::string toLower(const std::string& str);
    static std::vector<std::string> splitIntoWords(const std::string& line);
    static bool isWordChar(char c);
};

#endif
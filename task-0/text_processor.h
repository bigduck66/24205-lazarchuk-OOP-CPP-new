#ifndef TEXT_PROCESSOR_H
#define TEXT_PROCESSOR_H

#include <string>
#include <vector>

class TextProcessor {
public:
    std::vector<std::string> processLine(const std::string& line);
};

#endif
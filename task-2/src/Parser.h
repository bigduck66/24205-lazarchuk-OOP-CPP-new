#ifndef PARSER_H
#define PARSER_H

#include "GameConfig.h"
#include <string>
#include <exception>

class Parser {
public:
    GameConfig parse(const std::string& filename);
    GameConfig parseFromString(const std::string& content);
    
private:
    void parseFormat(const std::string& line, GameConfig& config);
    void parseName(const std::string& line, GameConfig& config);
    void parseRules(const std::string& line, GameConfig& config);
    void parseCoordinates(const std::string& line, GameConfig& config);
    void updateBounds(int x, int y, GameConfig& config);
};

class ParseException : public std::exception {
private:
    std::string message;
    
public:
    explicit ParseException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif
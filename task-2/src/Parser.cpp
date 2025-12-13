#include "Parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <climits>
#include <cctype>

GameConfig Parser::parse(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw ParseException("Cannot open file: " + filename);
    }
    
    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }
    
    return parseFromString(content);
}

GameConfig Parser::parseFromString(const std::string& content) {
    GameConfig config;
    std::istringstream stream(content);
    std::string line;
    
    config.minX = INT_MAX;
    config.maxX = INT_MIN;
    config.minY = INT_MAX;
    config.maxY = INT_MIN;
    
    config.birthRules = {3};
    config.survivalRules = {2, 3};
    config.rulesString = "B3/S23";
    
    bool hasFormat = false;
    
    while (std::getline(stream, line)) {
        if (line.empty()) continue;
        
        if (line[0] == '#') {
            if (line.substr(0, 8) == "#Life 1.") {
                parseFormat(line, config);
                hasFormat = true;
            } else if (line.substr(0, 3) == "#N ") {
                parseName(line, config);
            } else if (line.substr(0, 3) == "#R ") {
                parseRules(line, config);
            } else if (line.substr(0, 2) == "# ") {
                continue;
            } else {
                std::cout << "Warning: Unknown comment line: " << line << std::endl;
            }
        } else {
            parseCoordinates(line, config);
        }
    }
    
    if (!hasFormat) {
        throw ParseException("Invalid file format: Missing #Life header");
    }
    
    return config;
}

void Parser::parseFormat(const std::string& line, GameConfig& config) {
    config.format = line;
}

void Parser::parseName(const std::string& line, GameConfig& config) {
    config.name = line.substr(3);
}

void Parser::parseRules(const std::string& line, GameConfig& config) {
    std::string rulesStr = line.substr(3);
    
    size_t bPos = rulesStr.find('B');
    size_t sPos = rulesStr.find('S');
    size_t slashPos = rulesStr.find('/');
    
    if (bPos == std::string::npos || sPos == std::string::npos) {
        throw ParseException("Invalid rules format: Missing B or S indicator");
    }
    
    std::string birthStr;
    if (slashPos != std::string::npos) {
        birthStr = rulesStr.substr(bPos + 1, slashPos - bPos - 1);
    } else {
        birthStr = rulesStr.substr(bPos + 1, sPos - bPos - 1);
    }
    
    std::string survivalStr = rulesStr.substr(sPos + 1);
    
    config.birthRules.clear();
    for (char c : birthStr) {
        if (std::isdigit(c)) {
            config.birthRules.insert(c - '0');
        }
    }
    
    config.survivalRules.clear();
    for (char c : survivalStr) {
        if (std::isdigit(c)) {
            config.survivalRules.insert(c - '0');
        }
    }
    
    config.rulesString = rulesStr;
}

void Parser::parseCoordinates(const std::string& line, GameConfig& config) {
    std::istringstream iss(line);
    int x, y;
    
    if (iss >> x >> y) {
        config.coordinates.emplace_back(x, y);
        updateBounds(x, y, config);
    } else {
        throw ParseException("Invalid coordinate format: " + line);
    }
}

void Parser::updateBounds(int x, int y, GameConfig& config) {
    config.minX = std::min(config.minX, x);
    config.maxX = std::max(config.maxX, x);
    config.minY = std::min(config.minY, y);
    config.maxY = std::max(config.maxY, y);
}
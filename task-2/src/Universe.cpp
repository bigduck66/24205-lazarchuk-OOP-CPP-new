#include "Universe.h"
#include "Parser.h"
#include <fstream>
#include <iostream>

Universe::Universe(int w, int h, const std::string& universeName) 
    : width(w), height(h), name(universeName), generation(0) {
    grid.resize(height, std::vector<bool>(width, false));
    
    birthRules = {3};
    survivalRules = {2, 3};
}

Universe::Universe(const std::string& filename) {
    Parser parser;
    GameConfig config = parser.parse(filename);
    
    name = config.name;
    birthRules = config.birthRules;
    survivalRules = config.survivalRules;
    generation = 0;
    
    width = (config.maxX - config.minX + 1) + 4;
    height = (config.maxY - config.minY + 1) + 4;
    int offsetX = -config.minX + 2;
    int offsetY = -config.minY + 2;
    
    grid.resize(height, std::vector<bool>(width, false));
    
    for (const auto& coord : config.coordinates) {
        int x = coord.first + offsetX;
        int y = coord.second + offsetY;
        if (x >= 0 && x < width && y >= 0 && y < height) {
            grid[y][x] = true;
        }
    }
}

void Universe::setRules(const std::set<int>& birth, const std::set<int>& survival) {
    birthRules = birth;
    survivalRules = survival;
}

void Universe::setCell(int x, int y, bool state) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[y][x] = state;
    }
}

bool Universe::getCell(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return grid[y][x];
    }
    return false;
}

int Universe::countNeighbors(int x, int y) const {
    int count = 0;
    
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            
            int nx = (x + dx + width) % width;
            int ny = (y + dy + height) % height;
            
            if (grid[ny][nx]) {
                count++;
            }
        }
    }
    
    return count;
}

bool Universe::applyRules(bool currentState, int neighbors) const {
    if (currentState) {
        return survivalRules.find(neighbors) != survivalRules.end();
    } else {
        return birthRules.find(neighbors) != birthRules.end();
    }
}

void Universe::nextGeneration() {
    std::vector<std::vector<bool>> newGrid = grid;
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int neighbors = countNeighbors(x, y);
            newGrid[y][x] = applyRules(grid[y][x], neighbors);
        }
    }
    
    grid = newGrid;
    generation++;
}

void Universe::nextGenerations(int n) {
    for (int i = 0; i < n; ++i) {
        nextGeneration();
    }
}

void Universe::loadFromFile(const std::string& filename) {
    Parser parser;
    GameConfig config = parser.parse(filename);
    
    name = config.name;
    birthRules = config.birthRules;
    survivalRules = config.survivalRules;
    
    width = (config.maxX - config.minX + 1) + 4;
    height = (config.maxY - config.minY + 1) + 4;
    int offsetX = -config.minX + 2;
    int offsetY = -config.minY + 2;
    
    grid.clear();
    grid.resize(height, std::vector<bool>(width, false));
    
    for (const auto& coord : config.coordinates) {
        int x = coord.first + offsetX;
        int y = coord.second + offsetY;
        if (x >= 0 && x < width && y >= 0 && y < height) {
            grid[y][x] = true;
        }
    }
}

void Universe::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create file: " + filename);
    }
    
    file << "#Life 1.06\n";
    file << "#N " << name << "\n";
    file << "#R " << getRulesString() << "\n";
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x]) {
                file << x << " " << y << "\n";
            }
        }
    }
}

std::string Universe::getRulesString() const {
    std::string result = "B";
    for (int rule : birthRules) {
        result += std::to_string(rule);
    }
    result += "/S";
    for (int rule : survivalRules) {
        result += std::to_string(rule);
    }
    return result;
}

void Universe::clear() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid[y][x] = false;
        }
    }
    generation = 0;
}
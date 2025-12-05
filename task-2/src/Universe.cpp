#include "Universe.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>
#include <algorithm>
#include <climits> 

Universe::Universe(int w, int h, const std::string& universeName) 
    : width(w), height(h), name(universeName), generation(0) {
    grid.resize(height, std::vector<bool>(width, false));
    
    // Default rules: Conway's Game of Life
    birthRules = {3};
    survivalRules = {2, 3};
}

Universe::Universe(const std::string& filename) {
    loadFromFile(filename);
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
    
    // Check all 8 neighbors with toroidal wrapping
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
        // Cell is alive - check survival rules
        return survivalRules.find(neighbors) != survivalRules.end();
    } else {
        // Cell is dead - check birth rules
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
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    std::string line;
    generation = 0;
    
    // Read file format
    std::getline(file, line);
    if (line != "#Life 1.06") {
        std::cout << "Warning: File format may not be Life 1.06" << std::endl;
    }
    
    // Read name
    std::getline(file, line);
    if (line.substr(0, 3) == "#N ") {
        name = line.substr(3);
    } else {
        std::cout << "Warning: Universe name not found, using default" << std::endl;
        name = "Universe";
        // Put the line back for rules parsing
        file.seekg(-static_cast<int>(line.length()) - 1, std::ios_base::cur);
    }
    
    // Read rules
    std::getline(file, line);
    if (line.substr(0, 3) == "#R ") {
        std::string rulesStr = line.substr(3);
        size_t bPos = rulesStr.find('B');
        size_t sPos = rulesStr.find('S');
        size_t slashPos = rulesStr.find('/');
        
        if (bPos != std::string::npos && sPos != std::string::npos && slashPos != std::string::npos) {
            std::string birthStr = rulesStr.substr(bPos + 1, slashPos - bPos - 1);
            std::string survivalStr = rulesStr.substr(sPos + 1);
            
            birthRules.clear();
            survivalRules.clear();
            
            for (char c : birthStr) {
                if (isdigit(c)) {
                    birthRules.insert(c - '0');
                }
            }
            
            for (char c : survivalStr) {
                if (isdigit(c)) {
                    survivalRules.insert(c - '0');
                }
            }
        }
    } else {
        std::cout << "Warning: Rules not found, using Conway's Game of Life rules" << std::endl;
        birthRules = {3};
        survivalRules = {2, 3};
        // Put the line back for coordinate parsing
        file.seekg(-static_cast<int>(line.length()) - 1, std::ios_base::cur);
    }
    
    // Find min and max coordinates to determine field size
    int minX = INT_MAX, maxX = INT_MIN;
    int minY = INT_MAX, maxY = INT_MIN;
    std::vector<std::pair<int, int>> coordinates;
    
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        int x, y;
        if (iss >> x >> y) {
            coordinates.emplace_back(x, y);
            minX = std::min(minX, x);
            maxX = std::max(maxX, x);
            minY = std::min(minY, y);
            maxY = std::max(maxY, y);
        }
    }
    
    // Calculate dimensions with some padding
    width = (maxX - minX + 1) + 4;
    height = (maxY - minY + 1) + 4;
    int offsetX = -minX + 2;
    int offsetY = -minY + 2;
    
    grid.resize(height, std::vector<bool>(width, false));
    
    // Set living cells
    for (const auto& coord : coordinates) {
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

void Universe::randomize() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid[y][x] = dis(gen) == 1;
        }
    }
    generation = 0;
}

void Universe::clear() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid[y][x] = false;
        }
    }
    generation = 0;
}
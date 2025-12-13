#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <vector>
#include <string>
#include <set>

class Universe {
private:
    int width;
    int height;
    std::vector<std::vector<bool>> grid;
    std::set<int> birthRules;
    std::set<int> survivalRules;
    std::string name;
    int generation;

    int countNeighbors(int x, int y) const;
    bool applyRules(bool currentState, int neighbors) const;

public:
    Universe(int w, int h, const std::string& universeName = "Universe");
    Universe(const std::string& filename);
    
    void setRules(const std::set<int>& birth, const std::set<int>& survival);
    void setCell(int x, int y, bool state);
    bool getCell(int x, int y) const;
    void nextGeneration();
    void nextGenerations(int n);
    
    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getGeneration() const { return generation; }
    const std::string& getName() const { return name; }
    std::string getRulesString() const;
    
    void clear();
};

#endif
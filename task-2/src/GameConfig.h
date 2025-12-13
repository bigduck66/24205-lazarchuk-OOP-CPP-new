#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include <string>
#include <vector>
#include <set>
#include <utility>

struct GameConfig {
    std::string format;
    std::string name;
    std::string rulesString;
    std::set<int> birthRules;
    std::set<int> survivalRules;
    std::vector<std::pair<int, int>> coordinates;
    
    int minX = 0;
    int maxX = 0;
    int minY = 0;
    int maxY = 0;
    
    GameConfig() : format("#Life 1.06"), name("Universe") {
        birthRules = {3};
        survivalRules = {2, 3};
    }
};

#endif
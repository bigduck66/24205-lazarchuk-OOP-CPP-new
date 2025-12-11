#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include "Universe.h"
#include <string>

class GameOfLife {
private:
    Universe universe;
    bool running;
    
public:
    GameOfLife();
    GameOfLife(const std::string& filename);
    
    void run();
    void runOffline(const std::string& inputFile, const std::string& outputFile, int iterations);
    
    // Публичные методы для доступа командам
    void printUniverse() const;
    void showHelp() const;
    
    bool isRunning() const { return running; }
    void setRunning(bool value) { running = value; }
    
    Universe& getUniverse() { return universe; }
    const Universe& getUniverse() const { return universe; }
};

#endif
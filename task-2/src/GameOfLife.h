#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include "Universe.h"
#include <string>

class GameOfLife {
private:
    Universe universe;
    bool running;
    void printUniverse() const;
    void processCommand(const std::string& command);
    void showHelp() const;
public:
    GameOfLife();
    GameOfLife(const std::string& filename);
    
    void run();
    void runOffline(const std::string& inputFile, const std::string& outputFile, int iterations);
};

#endif
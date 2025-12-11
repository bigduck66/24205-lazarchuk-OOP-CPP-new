#include "GameOfLife.h"
#include "Command.h"
#include <iostream>
#include <memory>

GameOfLife::GameOfLife() : universe(40, 20, "Default Universe"), running(true) {
    universe.setCell(1, 0, true);
    universe.setCell(2, 1, true);
    universe.setCell(0, 2, true);
    universe.setCell(1, 2, true);
    universe.setCell(2, 2, true);
}

GameOfLife::GameOfLife(const std::string& filename) : universe(filename), running(true) {}

void GameOfLife::printUniverse() const {
    std::cout << "\nUniverse: " << universe.getName() << std::endl;
    std::cout << "Rules: " << universe.getRulesString() << std::endl;
    std::cout << "Generation: " << universe.getGeneration() << std::endl;
    std::cout << std::endl;
    
    for (int y = 0; y < universe.getHeight(); ++y) {
        for (int x = 0; x < universe.getWidth(); ++x) {
            std::cout << (universe.getCell(x, y) ? "■ " : "· ");
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void GameOfLife::showHelp() const {
    std::cout << "Available commands:\n";
    std::cout << "  help - show this help message\n";
    std::cout << "  tick [n] or t [n] - advance n generations (default: 1)\n";
    std::cout << "  dump <filename> - save universe to file\n";
    std::cout << "  exit - quit the game\n";
}

void GameOfLife::run() {
    std::cout << "Welcome to Game of Life!" << std::endl;
    showHelp();
    printUniverse();
    
    while (running) {
        std::cout << "> ";
        std::string commandStr;
        std::getline(std::cin, commandStr);
        
        if (commandStr.empty()) continue;
        
        try {
            std::unique_ptr<Command> command = CommandParser::parse(commandStr);
            command->execute(*this);
            
            std::string cmdName = command->getName();
            if (cmdName == "tick" || cmdName == "dump") {
                printUniverse();
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
            std::cout << "Type 'help' for available commands\n";
        }
    }
}

void GameOfLife::runOffline(const std::string& inputFile, const std::string& outputFile, int iterations) {
    try {
        Universe offlineUniverse(inputFile);
        offlineUniverse.nextGenerations(iterations);
        offlineUniverse.saveToFile(outputFile);
        std::cout << "Completed " << iterations << " iterations and saved to " << outputFile << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}
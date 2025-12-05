#include "GameOfLife.h"
#include <iostream>
#include <sstream>

GameOfLife::GameOfLife() : universe(40, 20, "Default Universe"), running(true) {
    // Create a glider as default pattern
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

void GameOfLife::processCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;
    
    if (cmd == "help") {
        showHelp();
    } else if (cmd == "tick" || cmd == "t") {
        int n = 1;
        if (iss >> n) {
            if (n > 0) {
                universe.nextGenerations(n);
                printUniverse();
            } else {
                std::cout << "Number of iterations must be positive\n";
            }
        } else {
            universe.nextGeneration();
            printUniverse();
        }
    } else if (cmd == "dump") {
        std::string filename;
        if (iss >> filename) {
            try {
                universe.saveToFile(filename);
                std::cout << "Universe saved to " << filename << std::endl;
            } catch (const std::exception& e) {
                std::cout << "Error saving file: " << e.what() << std::endl;
            }
        } else {
            std::cout << "Usage: dump <filename>\n";
        }
    } else if (cmd == "exit") {
        running = false;
        std::cout << "Goodbye!\n";
    } else {
        std::cout << "Unknown command. Type 'help' for available commands.\n";
    }
}

void GameOfLife::run() {
    std::cout << "Welcome to Game of Life!" << std::endl;
    showHelp();
    printUniverse();
    
    while (running) {
        std::cout << "> ";
        std::string command;
        std::getline(std::cin, command);
        
        if (command.empty()) continue;
        
        processCommand(command);
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
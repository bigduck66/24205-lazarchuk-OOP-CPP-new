#include "GameOfLife.h"
#include <iostream>
#include <string>

void printUsage() {
    std::cout << "Usage:\n";
    std::cout << "  gameoflife [input_file]                    - Interactive mode with optional input file\n";
    std::cout << "  gameoflife --input input_file --iterations n --output output_file  - Offline mode\n";
    std::cout << "  gameoflife -i n -o output_file input_file  - Alternative offline syntax\n";
}

int main(int argc, char* argv[]) {
    std::string inputFile;
    std::string outputFile;
    int iterations = 0;
    bool offlineMode = false;
    
    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-i" || arg == "--iterations") {
            if (i + 1 < argc) {
                iterations = std::stoi(argv[++i]);
                offlineMode = true;
            }
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                outputFile = argv[++i];
                offlineMode = true;
            }
        } else if (arg == "--input") {
            if (i + 1 < argc) {
                inputFile = argv[++i];
            }
        } else if (arg.substr(0, 2) != "--" && inputFile.empty()) {
            // Assume it's an input file
            inputFile = arg;
        }
    }
    
    try {
        if (offlineMode) {
            if (inputFile.empty() || outputFile.empty() || iterations <= 0) {
                std::cout << "Error: Offline mode requires input file, output file, and positive number of iterations\n";
                printUsage();
                return 1;
            }
            
            GameOfLife game;
            game.runOffline(inputFile, outputFile, iterations);
        } else {
            if (inputFile.empty()) {
                GameOfLife game;
                game.run();
            } else {
                GameOfLife game(inputFile);
                game.run();
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
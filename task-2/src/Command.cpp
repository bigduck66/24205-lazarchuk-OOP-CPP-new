#include "Command.h"
#include "GameOfLife.h"
#include <sstream>
#include <iostream>
#include <memory>
#include <cctype>

void HelpCommand::execute(GameOfLife& game) {
    game.showHelp();
}

void TickCommand::execute(GameOfLife& game) {
    if (iterations > 0) {
        game.getUniverse().nextGenerations(iterations);
    } else {
        std::cout << "Number of iterations must be positive\n";
    }
}

void DumpCommand::execute(GameOfLife& game) {
    try {
        game.getUniverse().saveToFile(filename);
        std::cout << "Universe saved to " << filename << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error saving file: " << e.what() << std::endl;
    }
}

void ExitCommand::execute(GameOfLife& game) {
    game.setRunning(false);
    std::cout << "Goodbye!\n";
}

std::unique_ptr<Command> CommandParser::parse(const std::string& commandStr) {
    std::istringstream iss(commandStr);//разбиваем строку на части
    std::string cmd;
    iss >> cmd;//извлекаем первое слово
    
    std::string lowerCmd;
    for (char c : cmd) {
        lowerCmd += std::tolower(c);//приводим к нижнему регистру
    }
    
    if (lowerCmd == "help") {
        return std::make_unique<HelpCommand>();//создаем HelpCommand
    }
    else if (lowerCmd == "tick" || lowerCmd == "t") {
        int n = 1;
        if (iss >> n) {
            return std::make_unique<TickCommand>(n);//TickCommand c числом
        } else {
            iss.clear();
            iss.seekg(0);
            std::string temp;
            iss >> temp;
            if (iss.eof()) {
                return std::make_unique<TickCommand>();
            } else {
                throw std::invalid_argument("Invalid number for tick command");
            }
        }
    }
    else if (lowerCmd == "dump") {
        std::string filename;
        if (iss >> filename) {
            return std::make_unique<DumpCommand>(filename);
        } else {
            throw std::invalid_argument("Missing filename for dump command");
        }
    }
    else if (lowerCmd == "exit") {
        return std::make_unique<ExitCommand>();
    }
    else {
        throw std::invalid_argument("Unknown command: " + cmd);
    }
}
#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <memory>

class GameOfLife;

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(GameOfLife& game) = 0;
    virtual std::string getName() const = 0;
};

class HelpCommand : public Command {
public:
    void execute(GameOfLife& game) override;
    std::string getName() const override { return "help"; }
};

class TickCommand : public Command {
private:
    int iterations;
    
public:
    explicit TickCommand(int n = 1) : iterations(n) {}
    void execute(GameOfLife& game) override;
    std::string getName() const override { return "tick"; }
    int getIterations() const { return iterations; }
};

class DumpCommand : public Command {
private:
    std::string filename;
    
public:
    explicit DumpCommand(const std::string& fname) : filename(fname) {}
    void execute(GameOfLife& game) override;
    std::string getName() const override { return "dump"; }
    std::string getFilename() const { return filename; }
};

class ExitCommand : public Command {
public:
    void execute(GameOfLife& game) override;
    std::string getName() const override { return "exit"; }
};

class CommandParser {
public:
    static std::unique_ptr<Command> parse(const std::string& commandStr);
};

#endif
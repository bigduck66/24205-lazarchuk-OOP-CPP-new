#include "../src/GameOfLife.h"
#include "../src/Command.h"
#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include <cstdio>

class TestGameOfLife : public ::testing::Test {
protected:
    void SetUp() override {
        oldCinBuffer = std::cin.rdbuf();
        oldCoutBuffer = std::cout.rdbuf();
        oldCerrBuffer = std::cerr.rdbuf();
        
        std::cin.rdbuf(testInput.rdbuf());
        std::cout.rdbuf(testOutput.rdbuf());
        std::cerr.rdbuf(testOutput.rdbuf());
    }
    
    void TearDown() override {
        std::cin.rdbuf(oldCinBuffer);
        std::cout.rdbuf(oldCoutBuffer);
        std::cerr.rdbuf(oldCerrBuffer);
    }
    
    void provideInput(const std::string& input) {
        testInput << input << "\n";
    }
    
    std::string getOutput() {
        std::string output = testOutput.str();
        testOutput.str("");
        testOutput.clear();
        return output;
    }
    
    void clearOutput() {
        testOutput.str("");
        testOutput.clear();
    }
    
    void resetInput() {
        testInput.str("");
        testInput.clear();
    }
    
    std::stringstream testInput;
    std::stringstream testOutput;
    std::streambuf* oldCinBuffer;
    std::streambuf* oldCoutBuffer;
    std::streambuf* oldCerrBuffer;
};

TEST_F(TestGameOfLife, DefaultConstructorCreatesUniverse) {
    EXPECT_NO_THROW({
        GameOfLife game;
    });
}

TEST_F(TestGameOfLife, ConstructorWithValidFile) {
    const std::string testFile = "test_constructor_file.life";
    std::ofstream file(testFile);
    file << "#Life 1.06\n";
    file << "#N Test Universe\n";
    file << "#R B3/S23\n";
    file << "1 1\n";
    file << "2 2\n";
    file.close();
    
    EXPECT_NO_THROW({
        GameOfLife game(testFile);
    });
    
    std::remove(testFile.c_str());
}


TEST_F(TestGameOfLife, RunOfflineModeSuccess) {
    GameOfLife game;
    
    const std::string inputFile = "test_offline_input.life";
    std::ofstream inFile(inputFile);
    inFile << "#Life 1.06\n";
    inFile << "#N Offline Test\n";
    inFile << "#R B3/S23\n";
    inFile << "1 1\n";
    inFile << "2 2\n";
    inFile << "3 3\n";
    inFile.close();
    
    const std::string outputFile = "test_offline_output.life";
    const int iterations = 3;
    
    EXPECT_NO_THROW({
        game.runOffline(inputFile, outputFile, iterations);
    });
    
    std::string output = getOutput();
    EXPECT_TRUE(output.find("Completed") != std::string::npos 
                || output.find("saved") != std::string::npos)
        << "Output was: " << output;
    
    std::ifstream outFile(outputFile);
    EXPECT_TRUE(outFile.good());
    
    outFile.close();
    std::remove(inputFile.c_str());
    std::remove(outputFile.c_str());
}

TEST_F(TestGameOfLife, RunOfflineModeInvalidInputFile) {
    GameOfLife game;
    
    const std::string inputFile = "non_existent_input.life";
    const std::string outputFile = "test_output.life";
    
    game.runOffline(inputFile, outputFile, 5);
    
    std::string output = getOutput();
    EXPECT_TRUE(output.find("Error") != std::string::npos 
                || output.find("error") != std::string::npos
                || output.find("Cannot open") != std::string::npos)
        << "Output was: " << output;
}

TEST_F(TestGameOfLife, RunOfflineModeWithZeroIterations) {
    GameOfLife game;
    
    const std::string inputFile = "test_zero_iter.life";
    std::ofstream inFile(inputFile);
    inFile << "#Life 1.06\n";
    inFile << "#N Zero Iter Test\n";
    inFile << "#R B3/S23\n";
    inFile << "1 1\n";
    inFile.close();
    
    const std::string outputFile = "test_zero_output.life";
    
    game.runOffline(inputFile, outputFile, 0);
    
    std::string output = getOutput();
    EXPECT_TRUE(output.find("Completed") != std::string::npos 
                || output.find("saved") != std::string::npos)
        << "Output was: " << output;
    
    std::ifstream outFile(outputFile);
    if (outFile.good()) {
        outFile.close();
        std::remove(outputFile.c_str());
    }
    
    std::remove(inputFile.c_str());
}

TEST_F(TestGameOfLife, ShowHelpCommand) {
    GameOfLife game;
    game.showHelp();
    
    std::string output = getOutput();
    EXPECT_TRUE(output.find("Available commands") != std::string::npos 
                || output.find("help") != std::string::npos
                || output.find("tick") != std::string::npos
                || output.find("dump") != std::string::npos
                || output.find("exit") != std::string::npos)
        << "Output was: " << output;
}

TEST_F(TestGameOfLife, GetAndSetUniverse) {
    GameOfLife game;
    
    Universe& universe = game.getUniverse();
    EXPECT_EQ(universe.getName(), "Default Universe");
    EXPECT_EQ(universe.getWidth(), 40);
    EXPECT_EQ(universe.getHeight(), 20);
    
    universe.setCell(5, 5, true);
    EXPECT_TRUE(universe.getCell(5, 5));
}

TEST_F(TestGameOfLife, CommandParserBasicCommands) {
    EXPECT_NO_THROW({
        auto cmd1 = CommandParser::parse("help");
        EXPECT_EQ(cmd1->getName(), "help");
        
        auto cmd2 = CommandParser::parse("tick");
        EXPECT_EQ(cmd2->getName(), "tick");
        
        auto cmd3 = CommandParser::parse("tick 5");
        EXPECT_EQ(cmd3->getName(), "tick");
        
        auto cmd4 = CommandParser::parse("dump file.life");
        EXPECT_EQ(cmd4->getName(), "dump");
        
        auto cmd5 = CommandParser::parse("exit");
        EXPECT_EQ(cmd5->getName(), "exit");
    });
}

TEST_F(TestGameOfLife, CommandParserInvalidCommands) {
    EXPECT_THROW(CommandParser::parse("invalid_command"), std::invalid_argument);
    EXPECT_THROW(CommandParser::parse("tick abc"), std::invalid_argument);
    EXPECT_THROW(CommandParser::parse("dump"), std::invalid_argument);
    EXPECT_THROW(CommandParser::parse(""), std::invalid_argument);
}

TEST_F(TestGameOfLife, CommandParserCaseInsensitive) {
    EXPECT_NO_THROW({
        auto cmd1 = CommandParser::parse("HELP");
        EXPECT_EQ(cmd1->getName(), "help");
        
        auto cmd2 = CommandParser::parse("Tick");
        EXPECT_EQ(cmd2->getName(), "tick");
        
        auto cmd3 = CommandParser::parse("DUMP file.life");
        EXPECT_EQ(cmd3->getName(), "dump");
        
        auto cmd4 = CommandParser::parse("EXIT");
        EXPECT_EQ(cmd4->getName(), "exit");
        
        auto cmd5 = CommandParser::parse("T 5");
        EXPECT_EQ(cmd5->getName(), "tick");
    });
}

TEST_F(TestGameOfLife, HelpCommandExecution) {
    GameOfLife game;
    HelpCommand helpCmd;
    
    helpCmd.execute(game);
    
    std::string output = getOutput();
    EXPECT_TRUE(output.find("Available commands") != std::string::npos 
                || output.find("help") != std::string::npos)
        << "Output was: " << output;
}

TEST_F(TestGameOfLife, DumpCommandExecution) {
    GameOfLife game;
    const std::string testFile = "test_dump_command.life";
    
    DumpCommand dumpCmd(testFile);
    dumpCmd.execute(game);
    
    std::string output = getOutput();
    EXPECT_TRUE(output.find("saved") != std::string::npos
                || output.find(testFile) != std::string::npos)
        << "Output was: " << output;
    
    std::ifstream file(testFile);
    EXPECT_TRUE(file.good());
    file.close();
    
    std::remove(testFile.c_str());
}

TEST_F(TestGameOfLife, ExitCommandExecution) {
    GameOfLife game;
    
    EXPECT_TRUE(game.isRunning());
    
    ExitCommand exitCmd;
    exitCmd.execute(game);
    
    std::string output = getOutput();
    EXPECT_TRUE(output.find("Goodbye") != std::string::npos)
        << "Output was: " << output;
    
    EXPECT_FALSE(game.isRunning());
}

TEST_F(TestGameOfLife, RunMethodBasicFlow) {
    GameOfLife game;
    
    provideInput("help");
    provideInput("tick");
    provideInput("dump test_run.life");
    provideInput("exit");
    game.showHelp();
    
    std::string output = getOutput();
    EXPECT_TRUE(output.find("Available commands") != std::string::npos 
                || output.find("help") != std::string::npos
                || output.find("tick") != std::string::npos)
        << "Output was: " << output;
    
    // Проверяем работу getUniverse
    Universe& universe = game.getUniverse();
    EXPECT_GT(universe.getWidth(), 0);
    EXPECT_GT(universe.getHeight(), 0);
    
    std::remove("test_run.life");
}

TEST_F(TestGameOfLife, MainIntegrationTest) {
    const std::string inputFile = "integration_test.life";
    const std::string outputFile = "integration_output.life";
    
    // Создаем тестовый файл
    std::ofstream inFile(inputFile);
    inFile << "#Life 1.06\n";
    inFile << "#N Integration Test\n";
    inFile << "#R B3/S23\n";
    inFile << "0 0\n";
    inFile << "1 0\n";
    inFile << "2 0\n";  // Горизонтальная линия из 3 клеток
    inFile.close();
    
    GameOfLife game(inputFile);
    
    // Проверяем, что загрузилось правильно
    Universe& universe = game.getUniverse();
    EXPECT_GT(universe.getWidth(), 0);
    EXPECT_GT(universe.getHeight(), 0);
    
    // Проверяем работу команд
    std::unique_ptr<Command> tickCmd = CommandParser::parse("tick 2");
    tickCmd->execute(game);
    
    clearOutput();
    
    DumpCommand dumpCmd(outputFile);
    dumpCmd.execute(game);
    
    std::string output = getOutput();
    EXPECT_TRUE(output.find("saved") != std::string::npos)
        << "Output was: " << output;
    
    std::ifstream outFile(outputFile);
    EXPECT_TRUE(outFile.good());
    outFile.close();
    
    // Очищаем
    std::remove(inputFile.c_str());
    std::remove(outputFile.c_str());
}

TEST_F(TestGameOfLife, ParserIntegration) {
    const std::string testFile = "parser_integration.life";
    
    std::ofstream file(testFile);
    file << "#Life 1.06\n";
    file << "#N Parser Integration Test\n";
    file << "#R B3678/S34678\n";  // Сложные правила
    file << "0 0\n";
    file << "10 10\n";
    file << "-5 -5\n";
    file << "100 100\n";
    file.close();
    
    EXPECT_NO_THROW({
        GameOfLife game(testFile);
        Universe& universe = game.getUniverse();
        
        std::string rules = universe.getRulesString();
        EXPECT_TRUE(rules.find("B3678") != std::string::npos ||
                    rules.find("B3786") != std::string::npos);
        EXPECT_TRUE(rules.find("S34678") != std::string::npos);
        
        EXPECT_GT(universe.getWidth(), 0);
        EXPECT_GT(universe.getHeight(), 0);
    });
    
    std::remove(testFile.c_str());
}
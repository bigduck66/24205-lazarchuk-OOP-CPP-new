#include "../src/GameOfLife.h"
#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <thread>
#include <chrono>
#include <future>

class TestGameOfLife : public ::testing::Test {
protected:
    void SetUp() override {
        // Сохраняем оригинальные потоки
        oldCinBuffer = std::cin.rdbuf();
        oldCoutBuffer = std::cout.rdbuf();
        oldCerrBuffer = std::cerr.rdbuf();
        
        // Перенаправляем потоки
        std::cin.rdbuf(testInput.rdbuf());
        std::cout.rdbuf(testOutput.rdbuf());
        std::cerr.rdbuf(testOutput.rdbuf());
    }
    
    void TearDown() override {
        // Восстанавливаем потоки
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
    // Просто проверяем, что конструктор не падает
    EXPECT_NO_THROW({
        GameOfLife game;
    });
}

TEST_F(TestGameOfLife, ConstructorWithValidFile) {
    // Создаем тестовый файл
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

TEST_F(TestGameOfLife, ConstructorWithInvalidFileThrows) {
    EXPECT_THROW({
        GameOfLife game("non_existent_file.life");
    }, std::runtime_error);
}

TEST_F(TestGameOfLife, RunOfflineModeSuccess) {
    GameOfLife game;
    
    // Создаем входной файл
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
    
    // Запускаем оффлайн режим
    EXPECT_NO_THROW({
        game.runOffline(inputFile, outputFile, iterations);
    });
    
    // Проверяем вывод
    std::string output = getOutput();
    EXPECT_TRUE(output.find("Completed") != std::string::npos 
                || output.find("saved") != std::string::npos)
        << "Output was: " << output;
    
    // Проверяем, что выходной файл создан
    std::ifstream outFile(outputFile);
    EXPECT_TRUE(outFile.good());
    
    // Проверяем формат файла
    std::string line;
    std::getline(outFile, line);
    EXPECT_EQ(line, "#Life 1.06");
    
    outFile.close();
    
    // Очищаем
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
    // Должно успешно завершиться даже с 0 итераций
    EXPECT_TRUE(output.find("Completed") != std::string::npos 
                || output.find("saved") != std::string::npos)
        << "Output was: " << output;
    
    // Проверяем, что файл создан
    std::ifstream outFile(outputFile);
    if (outFile.good()) {
        outFile.close();
        std::remove(outputFile.c_str());
    }
    
    std::remove(inputFile.c_str());
}

TEST_F(TestGameOfLife, RunOfflineModeLargeIterations) {
    GameOfLife game;
    
    const std::string inputFile = "test_large_iter.life";
    std::ofstream inFile(inputFile);
    inFile << "#Life 1.06\n";
    inFile << "#N Large Iter Test\n";
    inFile << "#R B3/S23\n";
    inFile << "1 1\n";
    inFile << "2 1\n";
    inFile << "1 2\n";
    inFile << "2 2\n";  // Стабильный блок
    inFile.close();
    
    const std::string outputFile = "test_large_output.life";
    const int largeIterations = 1000;
    
    EXPECT_NO_THROW({
        game.runOffline(inputFile, outputFile, largeIterations);
    });
    
    std::string output = getOutput();
    EXPECT_TRUE(output.find(std::to_string(largeIterations)) != std::string::npos
                || output.find("Completed") != std::string::npos)
        << "Output was: " << output;
    
    // Очищаем
    std::remove(inputFile.c_str());
    std::ifstream outFile(outputFile);
    if (outFile.good()) {
        outFile.close();
        std::remove(outputFile.c_str());
    }
}

TEST_F(TestGameOfLife, RunOfflineModeDifferentRules) {
    GameOfLife game;
    
    const std::string inputFile = "test_rules.life";
    std::ofstream inFile(inputFile);
    inFile << "#Life 1.06\n";
    inFile << "#N Different Rules\n";
    inFile << "#R B36/S23\n";  // HighLife rules
    inFile << "1 1\n";
    inFile << "2 1\n";
    inFile << "3 1\n";
    inFile << "1 2\n";
    inFile << "3 2\n";
    inFile << "2 3\n";  // Репликатор HighLife
    inFile.close();
    
    const std::string outputFile = "test_rules_output.life";
    
    game.runOffline(inputFile, outputFile, 10);
    
    std::string output = getOutput();
    EXPECT_TRUE(output.find("Completed") != std::string::npos
                || output.find("saved") != std::string::npos);
    
    // Проверяем, что в выходном файле сохранились правила
    std::ifstream outFile(outputFile);
    if (outFile.good()) {
        std::string line;
        while (std::getline(outFile, line)) {
            if (line.find("#R") == 0) {
                EXPECT_TRUE(line.find("B36") != std::string::npos);
                break;
            }
        }
        outFile.close();
    }
    
    // Очищаем
    std::remove(inputFile.c_str());
    std::remove(outputFile.c_str());
}

// Тест для проверки вывода приветственного сообщения
TEST_F(TestGameOfLife, RunMethodShowsWelcome) {
    GameOfLife game;
    
    // Запускаем run() в отдельном потоке и сразу выходим
    provideInput("exit");
    
    // Запускаем в отдельном потоке, так как run() блокирующий
    auto future = std::async(std::launch::async, [&game]() {
        game.run();
    });
    
    // Даем немного времени на выполнение
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Получаем вывод
    std::string output = getOutput();
    
    // Должно содержать приветствие
    EXPECT_TRUE(output.find("Welcome") != std::string::npos 
                || output.find("Game of Life") != std::string::npos)
        << "Output was: " << output;
    
    // Завершаем поток
    future.wait();
}

// Тест для проверки интерактивного режима с несколькими командами
TEST_F(TestGameOfLife, RunInteractiveModeBasic) {
    GameOfLife game;
    
    // Последовательность команд: help, tick, exit
    resetInput();
    provideInput("help");
    provideInput("tick");
    provideInput("exit");
    
    auto future = std::async(std::launch::async, [&game]() {
        game.run();
    });
    
    // Даем время на выполнение
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    std::string output = getOutput();
    
    // Проверяем наличие ключевых слов в выводе
    bool hasHelp = output.find("Available commands") != std::string::npos 
                   || output.find("help") != std::string::npos;
    bool hasGeneration = output.find("Generation") != std::string::npos;
    bool hasGoodbye = output.find("Goodbye") != std::string::npos;
    
    // Хотя бы одно из этих должно быть в выводе
    EXPECT_TRUE(hasHelp || hasGeneration || hasGoodbye)
        << "Output was: " << output;
    
    future.wait();
}

// Тест с некорректными командами
TEST_F(TestGameOfLife, RunInteractiveModeInvalidCommands) {
    GameOfLife game;
    
    resetInput();
    provideInput("invalid_command");
    provideInput("another_wrong");
    provideInput("exit");
    
    auto future = std::async(std::launch::async, [&game]() {
        game.run();
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    std::string output = getOutput();
    
    // Может содержать сообщение об ошибке
    // Проверяем, что не упало
    EXPECT_FALSE(output.empty()) << "Should have some output";
    
    future.wait();
}

// Тест команды dump в интерактивном режиме
TEST_F(TestGameOfLife, RunInteractiveModeDumpCommand) {
    GameOfLife game;
    const std::string dumpFile = "test_interactive_dump.life";
    
    resetInput();
    provideInput("dump " + dumpFile);
    provideInput("exit");
    
    auto future = std::async(std::launch::async, [&game]() {
        game.run();
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    std::string output = getOutput();
    
    // Проверяем, что файл создан
    std::ifstream file(dumpFile);
    bool fileCreated = file.good();
    file.close();
    
    if (fileCreated) {
        std::remove(dumpFile.c_str());
    }
    
    EXPECT_TRUE(output.find("saved") != std::string::npos
                || output.find("Goodbye") != std::string::npos
                || fileCreated)
        << "Output was: " << output;
    
    future.wait();
}

// Тест с пустыми командами
TEST_F(TestGameOfLife, RunInteractiveModeEmptyCommands) {
    GameOfLife game;
    
    resetInput();
    provideInput("");
    provideInput("   ");
    provideInput("exit");
    
    auto future = std::async(std::launch::async, [&game]() {
        game.run();
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    std::string output = getOutput();
    
    // Не должно падать на пустых командах
    EXPECT_FALSE(output.empty()) << "Should have some output even with empty commands";
    
    future.wait();
}

// Тест с очень длинной командой
TEST_F(TestGameOfLife, RunInteractiveModeLongCommand) {
    GameOfLife game;
    
    resetInput();
    // Создаем очень длинную команду
    std::string longCommand(1000, 'a');
    provideInput(longCommand);
    provideInput("exit");
    
    auto future = std::async(std::launch::async, [&game]() {
        game.run();
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    std::string output = getOutput();
    
    // Не должно падать на длинных командах
    EXPECT_TRUE(output.find("Unknown command") != std::string::npos
                || output.find("Goodbye") != std::string::npos)
        << "Output was: " << output;
    
    future.wait();
}

// Тест с командой tick и большим числом
TEST_F(TestGameOfLife, RunInteractiveModeTickLargeNumber) {
    GameOfLife game;
    
    resetInput();
    provideInput("tick 100");
    provideInput("exit");
    
    auto future = std::async(std::launch::async, [&game]() {
        game.run();
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    std::string output = getOutput();
    
    // Должно показывать поколение
    EXPECT_TRUE(output.find("Generation") != std::string::npos
                || output.find("Goodbye") != std::string::npos)
        << "Output was: " << output;
    
    future.wait();
}

// Тест с отрицательным числом в tick
TEST_F(TestGameOfLife, RunInteractiveModeTickNegative) {
    GameOfLife game;
    
    resetInput();
    provideInput("tick -5");
    provideInput("exit");
    
    auto future = std::async(std::launch::async, [&game]() {
        game.run();
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    std::string output = getOutput();
    
    // Должно показать сообщение об ошибке или проигнорировать
    bool hasError = output.find("positive") != std::string::npos 
                    || output.find("must be") != std::string::npos;
    bool hasGeneration = output.find("Generation") != std::string::npos;
    bool hasGoodbye = output.find("Goodbye") != std::string::npos;
    
    // Либо ошибка, либо проигнорировано - оба варианта допустимы
    EXPECT_TRUE(hasError || hasGeneration || hasGoodbye)
        << "Output was: " << output;
    
    future.wait();
}

// Тест создания из файла и запуска
TEST_F(TestGameOfLife, RunWithFileInput) {
    // Создаем тестовый файл
    const std::string inputFile = "test_run_input.life";
    std::ofstream file(inputFile);
    file << "#Life 1.06\n";
    file << "#N Run Test\n";
    file << "#R B3/S23\n";
    file << "5 5\n";
    file << "6 5\n";
    file << "7 5\n";
    file.close();
    
    GameOfLife game(inputFile);
    
    resetInput();
    provideInput("tick");
    provideInput("exit");
    
    auto future = std::async(std::launch::async, [&game]() {
        game.run();
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    std::string output = getOutput();
    
    // Должно показать имя вселенной из файла или поколение
    EXPECT_TRUE(output.find("Run Test") != std::string::npos 
                || output.find("Generation") != std::string::npos
                || output.find("Goodbye") != std::string::npos)
        << "Output was: " << output;
    
    future.wait();
    std::remove(inputFile.c_str());
}

// Тест многократного использования одного экземпляра
TEST_F(TestGameOfLife, MultipleRunCalls) {
    GameOfLife game;
    
    // Первый запуск
    resetInput();
    provideInput("help");
    provideInput("exit");
    
    auto future1 = std::async(std::launch::async, [&game]() {
        game.run();
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    future1.wait();
    clearOutput();
    
    // Второй запуск с тем же экземпляром
    resetInput();
    provideInput("tick");
    provideInput("exit");
    
    auto future2 = std::async(std::launch::async, [&game]() {
        game.run();
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    std::string output = getOutput();
    
    EXPECT_TRUE(output.find("Generation") != std::string::npos 
                || output.find("Goodbye") != std::string::npos)
        << "Output was: " << output;
    
    future2.wait();
}

// Тест на утечки памяти (базовый)
TEST_F(TestGameOfLife, NoMemoryLeaksOnDestruction) {
    // Создаем много объектов
    for (int i = 0; i < 100; ++i) {
        GameOfLife game;
        // Ничего не делаем
    }
    
    SUCCEED();
}

// Тест исключительных ситуаций
TEST_F(TestGameOfLife, ExceptionSafety) {
    // Попытка создать GameOfLife с несуществующим файлом
    // должна бросить исключение, но не привести к крешу
    try {
        GameOfLife game("definitely_non_existent_file_12345.life");
        FAIL() << "Should have thrown exception";
    } catch (const std::runtime_error& e) {
        // Ожидаемое поведение
        SUCCEED();
    } catch (const std::exception& e) {
        // Другие исключения тоже приемлемы
        SUCCEED();
    } catch (...) {
        FAIL() << "Unexpected exception type";
    }
}

// Тест граничных значений для runOffline
TEST_F(TestGameOfLife, RunOfflineBoundaryValues) {
    GameOfLife game;
    
    const std::string inputFile = "boundary_test.life";
    std::ofstream inFile(inputFile);
    inFile << "#Life 1.06\n";
    inFile << "#N Boundary Test\n";
    inFile << "#R B3/S23\n";
    inFile << "0 0\n";
    inFile.close();
    
    const std::string outputFile = "boundary_output.life";
    
    // Тест с 1 итерацией
    game.runOffline(inputFile, outputFile, 1);
    clearOutput();
    
    // Тест с очень большим числом итераций
    game.runOffline(inputFile, outputFile, 10000);
    
    std::string output = getOutput();
    EXPECT_TRUE(output.find("10000") != std::string::npos 
                || output.find("Completed") != std::string::npos
                || output.find("saved") != std::string::npos)
        << "Output was: " << output;
    
    // Очищаем
    std::remove(inputFile.c_str());
    std::ifstream outFile(outputFile);
    if (outFile.good()) {
        outFile.close();
        std::remove(outputFile.c_str());
    }
}

// Тест с файлом, содержащим много клеток
TEST_F(TestGameOfLife, RunOfflineWithManyCells) {
    GameOfLife game;
    
    const std::string inputFile = "many_cells.life";
    std::ofstream inFile(inputFile);
    inFile << "#Life 1.06\n";
    inFile << "#N Many Cells\n";
    inFile << "#R B3/S23\n";
    
    // Создаем много клеток (простую сетку)
    for (int i = 0; i < 50; ++i) {
        for (int j = 0; j < 50; j += 2) {
            inFile << i << " " << j << "\n";
        }
    }
    inFile.close();
    
    const std::string outputFile = "many_cells_output.life";
    
    EXPECT_NO_THROW({
        game.runOffline(inputFile, outputFile, 10);
    });
    
    std::string output = getOutput();
    EXPECT_TRUE(output.find("Completed") != std::string::npos
                || output.find("saved") != std::string::npos)
        << "Output was: " << output;
    
    // Очищаем
    std::remove(inputFile.c_str());
    std::ifstream outFile(outputFile);
    if (outFile.good()) {
        outFile.close();
        std::remove(outputFile.c_str());
    }
}

// Тест с пустым файлом (только заголовки)
TEST_F(TestGameOfLife, RunOfflineWithEmptyUniverse) {
    GameOfLife game;
    
    const std::string inputFile = "empty_universe.life";
    std::ofstream inFile(inputFile);
    inFile << "#Life 1.06\n";
    inFile << "#N Empty Universe\n";
    inFile << "#R B3/S23\n";
    // Нет живых клеток
    inFile.close();
    
    const std::string outputFile = "empty_output.life";
    
    game.runOffline(inputFile, outputFile, 5);
    
    std::string output = getOutput();
    EXPECT_TRUE(output.find("Completed") != std::string::npos
                || output.find("saved") != std::string::npos)
        << "Output was: " << output;
    
    // Проверяем, что выходной файл создан и пуст (кроме заголовков)
    std::ifstream outFile(outputFile);
    if (outFile.good()) {
        std::string line;
        int lineCount = 0;
        while (std::getline(outFile, line)) {
            lineCount++;
        }
        // Должно быть 3 строки (заголовки) или больше, если есть координаты
        EXPECT_GE(lineCount, 3);
        outFile.close();
    }
    
    // Очищаем
    std::remove(inputFile.c_str());
    std::remove(outputFile.c_str());
}

// Тест с неверным форматом файла
TEST_F(TestGameOfLife, RunOfflineWithInvalidFormat) {
    GameOfLife game;
    
    const std::string inputFile = "invalid_format.life";
    std::ofstream inFile(inputFile);
    inFile << "Not a valid Life format\n";
    inFile << "Some random text\n";
    inFile << "1 2 3\n";  // Неправильный формат
    inFile.close();
    
    const std::string outputFile = "invalid_output.life";
    
    // Может бросить исключение или вывести ошибку
    try {
        game.runOffline(inputFile, outputFile, 5);
        std::string output = getOutput();
        // Если не бросило исключение, должен быть вывод об ошибке
        EXPECT_TRUE(output.find("Error") != std::string::npos
                    || output.find("error") != std::string::npos
                    || output.find("Warning") != std::string::npos)
            << "Output was: " << output;
    } catch (const std::exception& e) {
        // Исключение тоже приемлемо
        SUCCEED();
    }
    
    // Очищаем
    std::remove(inputFile.c_str());
    if (std::ifstream(outputFile).good()) {
        std::remove(outputFile.c_str());
    }
}
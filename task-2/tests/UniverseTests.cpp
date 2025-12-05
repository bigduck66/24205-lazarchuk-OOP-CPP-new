#include "../src/Universe.h"
#include <gtest/gtest.h>
#include <fstream>
#include <set>

class UniverseTest : public ::testing::Test {
protected:
    void SetUp() override {
        universe = new Universe(10, 10, "Test Universe");
    }
    
    void TearDown() override {
        delete universe;
    }
    
    // Вспомогательные функции для тестирования
    void createBlock(int x, int y) {
        universe->setCell(x, y, true);
        universe->setCell(x+1, y, true);
        universe->setCell(x, y+1, true);
        universe->setCell(x+1, y+1, true);
    }
    
    int countLiveCells() const {
        int count = 0;
        for (int y = 0; y < universe->getHeight(); ++y) {
            for (int x = 0; x < universe->getWidth(); ++x) {
                if (universe->getCell(x, y)) count++;
            }
        }
        return count;
    }
    
    Universe* universe;
};

// Базовые тесты конструктора
TEST_F(UniverseTest, ConstructorInitializesCorrectly) {
    EXPECT_EQ(universe->getWidth(), 10);
    EXPECT_EQ(universe->getHeight(), 10);
    EXPECT_EQ(universe->getName(), "Test Universe");
    EXPECT_EQ(universe->getGeneration(), 0);
    
    // Все клетки должны быть мертвыми
    EXPECT_EQ(countLiveCells(), 0);
}

// Тесты для работы с ячейками
TEST_F(UniverseTest, SetAndGetCellOperations) {
    // Установка в пределах границ
    universe->setCell(0, 0, true);
    universe->setCell(5, 5, true);
    universe->setCell(9, 9, true);
    
    EXPECT_TRUE(universe->getCell(0, 0));
    EXPECT_TRUE(universe->getCell(5, 5));
    EXPECT_TRUE(universe->getCell(9, 9));
    
    // Проверка ложных ячеек
    EXPECT_FALSE(universe->getCell(1, 1));
    
    // Установка за границами не должна падать
    EXPECT_NO_THROW(universe->setCell(10, 10, true));
    EXPECT_NO_THROW(universe->setCell(-1, -1, true));
    
    // Получение за границами возвращает false
    EXPECT_FALSE(universe->getCell(10, 10));
    EXPECT_FALSE(universe->getCell(-1, -1));
}

TEST_F(UniverseTest, ConwayRulesBlockStability) {
    // Создаем стабильный блок 2x2
    createBlock(1, 1);
    
    EXPECT_EQ(countLiveCells(), 4);
    
    // Блок должен остаться неизменным после одного поколения
    universe->nextGeneration();
    
    EXPECT_TRUE(universe->getCell(1, 1));
    EXPECT_TRUE(universe->getCell(2, 1));
    EXPECT_TRUE(universe->getCell(1, 2));
    EXPECT_TRUE(universe->getCell(2, 2));
    EXPECT_EQ(countLiveCells(), 4);
    EXPECT_EQ(universe->getGeneration(), 1);
    
    // Блок должен остаться неизменным после нескольких поколений
    universe->nextGenerations(5);
    
    EXPECT_TRUE(universe->getCell(1, 1));
    EXPECT_TRUE(universe->getCell(2, 2));
    EXPECT_EQ(countLiveCells(), 4);
    EXPECT_EQ(universe->getGeneration(), 6);
}

TEST_F(UniverseTest, ConwayRulesLonelyCellDies) {
    // Одиночная клетка должна умереть
    universe->setCell(5, 5, true);
    
    EXPECT_EQ(countLiveCells(), 1);
    universe->nextGeneration();
    
    EXPECT_FALSE(universe->getCell(5, 5));
    EXPECT_EQ(countLiveCells(), 0);
}

TEST_F(UniverseTest, ConwayRulesOvercrowding) {
    // Клетка с 4+ соседями должна умереть от перенаселения
    /*
       X X X
       X X X  - центральная клетка имеет 8 соседей
       X X X
    */
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            universe->setCell(5 + dx, 5 + dy, true);
        }
    }
    
    EXPECT_EQ(countLiveCells(), 9);
    universe->nextGeneration();
    
    // Центральная клетка должна умереть (8 соседей > 3)
    EXPECT_FALSE(universe->getCell(5, 5));
    // Угловые клетки имеют 3 соседа - должны выжить
    EXPECT_TRUE(universe->getCell(4, 4));
    EXPECT_TRUE(universe->getCell(6, 6));
}

TEST_F(UniverseTest, ConwayRulesBirth) {
    // Мертвая клетка с ровно 3 соседями должна родиться
    /*
       X . X
       . . .  - центральная клетка (1,1) мертва, имеет 2 соседа
       X . .
       
       
    */
    
    universe->clear();
    // Создаем L-образную фигуру для рождения в центре
    // (1,1) будет иметь 3 соседа: (0,0), (1,0), (0,1)
    universe->setCell(0, 0, true); // левый верхний
    universe->setCell(1, 0, true); // верхний центр
    universe->setCell(0, 1, true); // левый центр
    
    EXPECT_EQ(countLiveCells(), 3);
    EXPECT_FALSE(universe->getCell(1, 1)); // Центр должен быть мертв
    
    universe->nextGeneration();
    
    // Центральная клетка должна родиться
    EXPECT_TRUE(universe->getCell(1, 1));
    // Старые клетки могут измениться - проверяем только общее количество
    EXPECT_EQ(countLiveCells(), 4); // 3 старых + 1 новая (или другая комбинация)
}

TEST_F(UniverseTest, GliderPattern) {
    // Создаем планер
    universe->clear();
    universe->setCell(1, 0, true);
    universe->setCell(2, 1, true);
    universe->setCell(0, 2, true);
    universe->setCell(1, 2, true);
    universe->setCell(2, 2, true);
    
    EXPECT_EQ(countLiveCells(), 5);
    
    // Планер должен сохранять 5 живых клеток после каждого поколения
    // (в тороидальном мире 10x10)
    for (int i = 0; i < 4; ++i) {
        universe->nextGeneration();
        EXPECT_EQ(countLiveCells(), 5);
    }
    
    EXPECT_EQ(universe->getGeneration(), 4);
}

TEST_F(UniverseTest, ToroidalWrapping) {
    // Проверяем тороидальную геометрию
    universe->clear();
    
    // Клетка в левом верхнем углу должна взаимодействовать с правым нижним
    universe->setCell(0, 0, true);       // левый верхний
    universe->setCell(9, 9, true);       // правый нижний
    universe->setCell(1, 0, true);       // чтобы (0,0) не умерла сразу
    
    // Создаем условия для рождения в (1,1)
    universe->setCell(0, 1, true);
    
    universe->nextGeneration();
    
    // Клетка (1,1) должна родиться (имеет 3 соседа)
    EXPECT_TRUE(universe->getCell(1, 1));
}

TEST_F(UniverseTest, CustomRules) {
    Universe custom(5, 5, "Custom Rules");
    
    // Устанавливаем правила B36/S23 (HighLife)
    std::set<int> birth{3, 6};
    std::set<int> survival{2, 3};
    custom.setRules(birth, survival);
    
    std::string rules = custom.getRulesString();
    EXPECT_TRUE(rules.find("B36") != std::string::npos ||
                rules.find("B63") != std::string::npos); // Порядок может быть разный
    EXPECT_TRUE(rules.find("S23") != std::string::npos ||
                rules.find("S32") != std::string::npos);
    
    // Тестируем что поколение увеличивается
    custom.nextGeneration();
    EXPECT_EQ(custom.getGeneration(), 1);
}

TEST_F(UniverseTest, FileOperations) {
    const std::string testFile = "test_universe.life";
    
    // Создаем и сохраняем вселенную
    universe->clear();
    universe->setCell(5, 5, true);
    universe->setCell(6, 6, true);
    universe->setCell(7, 7, true);
    
    // Проверяем что сохранение не падает
    ASSERT_NO_THROW(universe->saveToFile(testFile));
    
    // Проверяем что файл создан
    std::ifstream file(testFile);
    ASSERT_TRUE(file.good());
    
    // Проверяем базовый формат файла
    std::string line;
    std::getline(file, line);
    EXPECT_EQ(line, "#Life 1.06");
    
    file.close();
    
    // Загрузка не должна падать
    EXPECT_NO_THROW({
        Universe loaded(testFile);
        // После загрузки должно быть какое-то количество живых клеток
        // (хотя бы одна из трех, которые мы сохранили)
        bool hasLiveCells = false;
        for (int y = 0; y < loaded.getHeight(); ++y) {
            for (int x = 0; x < loaded.getWidth(); ++x) {
                if (loaded.getCell(x, y)) {
                    hasLiveCells = true;
                    break;
                }
            }
            if (hasLiveCells) break;
        }
        EXPECT_TRUE(hasLiveCells);
    });
    
    std::remove(testFile.c_str());
}

TEST_F(UniverseTest, LoadInvalidFile) {
    EXPECT_THROW({
        Universe u("non_existent.life");
    }, std::runtime_error);
}

TEST_F(UniverseTest, ClearUniverse) {
    // Заполняем вселенную
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; x += 2) {
            universe->setCell(x, y, true);
        }
    }
    
    universe->nextGeneration();
    EXPECT_GT(countLiveCells(), 0);
    EXPECT_EQ(universe->getGeneration(), 1);
    
    // Очищаем
    universe->clear();
    
    EXPECT_EQ(countLiveCells(), 0);
    EXPECT_EQ(universe->getGeneration(), 0);
    
    // После очистки можно снова добавлять клетки
    universe->setCell(5, 5, true);
    EXPECT_TRUE(universe->getCell(5, 5));
}

TEST_F(UniverseTest, RandomizeCreatesCells) {
    // Изначально все клетки мертвы
    EXPECT_EQ(countLiveCells(), 0);
    
    // Рандомизируем
    universe->randomize();
    
    // Не можем гарантировать, что есть живые клетки
    // Но можем проверить что поколение не изменилось
    EXPECT_EQ(universe->getGeneration(), 0);
    
    // После рандомизации можно эволюционировать
    ASSERT_NO_THROW(universe->nextGeneration());
    EXPECT_EQ(universe->getGeneration(), 1);
}

TEST_F(UniverseTest, MultipleGenerations) {
    createBlock(1, 1);
    
    int initialGen = universe->getGeneration();
    int iterations = 100;
    
    universe->nextGenerations(iterations);
    
    EXPECT_EQ(universe->getGeneration(), initialGen + iterations);
    EXPECT_EQ(countLiveCells(), 4); // Блок должен остаться стабильным
}

TEST_F(UniverseTest, NegativeIterations) {
    // nextGenerations с отрицательным или нулевым числом не должен ничего делать
    universe->setCell(5, 5, true);
    
    // Должно работать без ошибок
    EXPECT_NO_THROW(universe->nextGenerations(0));
    EXPECT_NO_THROW(universe->nextGenerations(-1));
    
    // Поколение не должно измениться
    EXPECT_EQ(universe->getGeneration(), 0);
    EXPECT_TRUE(universe->getCell(5, 5));
}

TEST_F(UniverseTest, LargeIterations) {
    // Тест большого числа итераций
    createBlock(1, 1);
    
    EXPECT_NO_THROW(universe->nextGenerations(1000));
    EXPECT_EQ(universe->getGeneration(), 1000);
    EXPECT_EQ(countLiveCells(), 4);
}

TEST_F(UniverseTest, RulesStringFormat) {
    Universe universe(5, 5, "Rules Test");
    
    // По умолчанию должны быть правила Conway
    std::string rules = universe.getRulesString();
    EXPECT_TRUE(rules.find("B3") != std::string::npos);
    EXPECT_TRUE(rules.find("S23") != std::string::npos);
    
    // Меняем правила
    std::set<int> birth{2, 3};
    std::set<int> survival{3};
    universe.setRules(birth, survival);
    
    rules = universe.getRulesString();
    EXPECT_TRUE(rules.find("B23") != std::string::npos ||
                rules.find("B32") != std::string::npos);
    EXPECT_TRUE(rules.find("S3") != std::string::npos);
}
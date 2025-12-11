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

TEST_F(UniverseTest, ConstructorInitializesCorrectly) {
    EXPECT_EQ(universe->getWidth(), 10);
    EXPECT_EQ(universe->getHeight(), 10);
    EXPECT_EQ(universe->getName(), "Test Universe");
    EXPECT_EQ(universe->getGeneration(), 0);
    EXPECT_EQ(countLiveCells(), 0);
}

TEST_F(UniverseTest, SetAndGetCellOperations) {
    universe->setCell(0, 0, true);
    universe->setCell(5, 5, true);
    universe->setCell(9, 9, true);
    
    EXPECT_TRUE(universe->getCell(0, 0));
    EXPECT_TRUE(universe->getCell(5, 5));
    EXPECT_TRUE(universe->getCell(9, 9));
    EXPECT_FALSE(universe->getCell(1, 1));
    
    EXPECT_NO_THROW(universe->setCell(10, 10, true));
    EXPECT_NO_THROW(universe->setCell(-1, -1, true));
    
    EXPECT_FALSE(universe->getCell(10, 10));
    EXPECT_FALSE(universe->getCell(-1, -1));
}

TEST_F(UniverseTest, ConwayRulesBlockStability) {
    createBlock(1, 1);
    EXPECT_EQ(countLiveCells(), 4);
    
    universe->nextGeneration();
    EXPECT_TRUE(universe->getCell(1, 1));
    EXPECT_TRUE(universe->getCell(2, 1));
    EXPECT_TRUE(universe->getCell(1, 2));
    EXPECT_TRUE(universe->getCell(2, 2));
    EXPECT_EQ(countLiveCells(), 4);
    EXPECT_EQ(universe->getGeneration(), 1);
    
    universe->nextGenerations(5);
    EXPECT_TRUE(universe->getCell(1, 1));
    EXPECT_TRUE(universe->getCell(2, 2));
    EXPECT_EQ(countLiveCells(), 4);
    EXPECT_EQ(universe->getGeneration(), 6);
}

TEST_F(UniverseTest, ConwayRulesLonelyCellDies) {
    universe->setCell(5, 5, true);
    EXPECT_EQ(countLiveCells(), 1);
    
    universe->nextGeneration();
    EXPECT_FALSE(universe->getCell(5, 5));
    EXPECT_EQ(countLiveCells(), 0);
}

TEST_F(UniverseTest, ClearUniverse) {
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; x += 2) {
            universe->setCell(x, y, true);
        }
    }
    
    universe->nextGeneration();
    EXPECT_GT(countLiveCells(), 0);
    EXPECT_EQ(universe->getGeneration(), 1);
    
    universe->clear();
    EXPECT_EQ(countLiveCells(), 0);
    EXPECT_EQ(universe->getGeneration(), 0);
    
    universe->setCell(5, 5, true);
    EXPECT_TRUE(universe->getCell(5, 5));
}

TEST_F(UniverseTest, MultipleGenerations) {
    createBlock(1, 1);
    
    int initialGen = universe->getGeneration();
    int iterations = 100;
    
    universe->nextGenerations(iterations);
    EXPECT_EQ(universe->getGeneration(), initialGen + iterations);
    EXPECT_EQ(countLiveCells(), 4);
}

TEST_F(UniverseTest, NegativeIterations) {
    universe->setCell(5, 5, true);
    
    EXPECT_NO_THROW(universe->nextGenerations(0));
    EXPECT_NO_THROW(universe->nextGenerations(-1));
    
    EXPECT_EQ(universe->getGeneration(), 0);
    EXPECT_TRUE(universe->getCell(5, 5));
}

TEST_F(UniverseTest, LargeIterations) {
    createBlock(1, 1);
    
    EXPECT_NO_THROW(universe->nextGenerations(1000));
    EXPECT_EQ(universe->getGeneration(), 1000);
    EXPECT_EQ(countLiveCells(), 4);
}

TEST_F(UniverseTest, RulesStringFormat) {
    Universe universe(5, 5, "Rules Test");
    
    std::string rules = universe.getRulesString();
    EXPECT_TRUE(rules.find("B3") != std::string::npos);
    EXPECT_TRUE(rules.find("S23") != std::string::npos);
    
    std::set<int> birth{2, 3};
    std::set<int> survival{3};
    universe.setRules(birth, survival);
    
    rules = universe.getRulesString();
    EXPECT_TRUE(rules.find("B23") != std::string::npos ||
                rules.find("B32") != std::string::npos);
    EXPECT_TRUE(rules.find("S3") != std::string::npos);
}

TEST_F(UniverseTest, FileOperations) {
    const std::string testFile = "test_universe.life";
    
    universe->clear();
    universe->setCell(5, 5, true);
    universe->setCell(6, 6, true);
    universe->setCell(7, 7, true);
    
    ASSERT_NO_THROW(universe->saveToFile(testFile));
    
    std::ifstream file(testFile);
    ASSERT_TRUE(file.good());
    
    std::string line;
    std::getline(file, line);
    EXPECT_EQ(line, "#Life 1.06");
    
    file.close();
    
    EXPECT_NO_THROW({
        Universe loaded(testFile);
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

TEST_F(UniverseTest, ToroidalWrapping) {
    universe->clear();
    
    universe->setCell(0, 0, true);
    universe->setCell(9, 9, true);
    universe->setCell(1, 0, true);
    universe->setCell(0, 1, true);
    
    universe->nextGeneration();
    EXPECT_TRUE(universe->getCell(1, 1));
}

TEST_F(UniverseTest, GliderPattern) {
    universe->clear();
    universe->setCell(1, 0, true);
    universe->setCell(2, 1, true);
    universe->setCell(0, 2, true);
    universe->setCell(1, 2, true);
    universe->setCell(2, 2, true);
    
    EXPECT_EQ(countLiveCells(), 5);
    
    for (int i = 0; i < 4; ++i) {
        universe->nextGeneration();
        EXPECT_EQ(countLiveCells(), 5);
    }
    
    EXPECT_EQ(universe->getGeneration(), 4);
}

TEST_F(UniverseTest, LoadFromFileWithParser) {
    const std::string testFile = "test_parser.life";
    
    std::ofstream file(testFile);
    file << "#Life 1.06\n";
    file << "#N Parser Test Universe\n";
    file << "#R B3/S23\n";
    file << "1 1\n";
    file << "2 2\n";
    file << "3 3\n";
    file.close();
    
    EXPECT_NO_THROW({
        Universe loaded(testFile);
        EXPECT_EQ(loaded.getName(), "Parser Test Universe");
        EXPECT_EQ(loaded.getRulesString(), "B3/S23");
    });
    
    std::remove(testFile.c_str());
}

TEST_F(UniverseTest, LoadFileWithCustomRules) {
    const std::string testFile = "test_custom_rules.life";
    
    std::ofstream file(testFile);
    file << "#Life 1.06\n";
    file << "#N Custom Rules\n";
    file << "#R B36/S23\n";  // HighLife rules
    file << "1 1\n";
    file.close();
    
    EXPECT_NO_THROW({
        Universe loaded(testFile);
        std::string rules = loaded.getRulesString();
        EXPECT_TRUE(rules.find("B36") != std::string::npos);
        EXPECT_TRUE(rules.find("S23") != std::string::npos);
    });
    
    std::remove(testFile.c_str());
}
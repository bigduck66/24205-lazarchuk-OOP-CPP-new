CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -I.
GTEST_FLAGS = -lgtest -lgtest_main -lpthread

# Основная программа
SOURCES = main.cpp file_reader.cpp text_processor.cpp word_analyzer.cpp csv_writer.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Тесты
TEST_SOURCES = test_word_counter.cpp file_reader.cpp text_processor.cpp word_analyzer.cpp csv_writer.cpp
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)

TARGET = word_count
TEST_TARGET = test_word_counter

.PHONY: all test clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SOURCES)
	$(CXX) $(CXXFLAGS) $(TEST_SOURCES) -o $(TEST_TARGET) $(GTEST_FLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(TEST_TARGET) *.o test_output.csv

# Быстрая компиляция тестов
quick-test:
	$(CXX) $(CXXFLAGS) test_word_counter.cpp file_reader.cpp text_processor.cpp word_analyzer.cpp csv_writer.cpp -o $(TEST_TARGET) $(GTEST_FLAGS)
	./$(TEST_TARGET)
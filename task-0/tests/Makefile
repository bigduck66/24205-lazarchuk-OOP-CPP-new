CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -I../src
GTEST_FLAGS = -lgtest -lgtest_main -lpthread

TEST_SOURCES = test_word_counter.cpp ../src/file_reader.cpp ../src/text_processor.cpp ../src/word_analyzer.cpp ../src/csv_writer.cpp
TEST_TARGET = test_word_counter

.PHONY: test clean

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SOURCES)
	$(CXX) $(CXXFLAGS) $(TEST_SOURCES) -o $(TEST_TARGET) $(GTEST_FLAGS)

clean:
	rm -f $(TEST_TARGET) *.o test_output.csv

# Быстрая компиляция тестов
quick-test:
	$(CXX) $(CXXFLAGS) test_word_counter.cpp ../src/file_reader.cpp ../src/text_processor.cpp ../src/word_analyzer.cpp ../src/csv_writer.cpp -o $(TEST_TARGET) $(GTEST_FLAGS)
	./$(TEST_TARGET)
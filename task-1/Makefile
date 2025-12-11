CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./src
MAIN_TARGET = program
TEST_TARGET = test_program
MAIN_SOURCES = src/BitArray.cpp src/main.cpp
TEST_SOURCES = src/BitArray.cpp tests/test_bitarray.cpp

# Заголовочные файлы
HEADERS = src/BitArray.h

# По умолчанию компилирует и запускает основную программу
default: run

# Основная программа
$(MAIN_TARGET): $(MAIN_SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(MAIN_TARGET) $(MAIN_SOURCES)

# Тесты
$(TEST_TARGET): $(TEST_SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_SOURCES)

# Запуск основной программы
run: $(MAIN_TARGET)
	./$(MAIN_TARGET)

# Запуск тестов
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Очистка
clean:
	rm -f $(MAIN_TARGET) $(TEST_TARGET)

.PHONY: clean run test default
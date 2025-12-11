CXX = g++
CXXFLAGS = -std=c++17 -I./src -Wall -Wextra
TESTFLAGS = -lgtest -lgtest_main -lpthread

# Исходные файлы
SRC_DIR = src
TEST_DIR = tests

# Основные исходники
MAIN_SOURCES = $(SRC_DIR)/main.cpp \
               $(SRC_DIR)/GameOfLife.cpp \
               $(SRC_DIR)/Universe.cpp \
               $(SRC_DIR)/Parser.cpp \
               $(SRC_DIR)/Command.cpp

# Тесты
UNIVERSE_TEST_SOURCES = $(TEST_DIR)/UniverseTests.cpp \
                        $(SRC_DIR)/Universe.cpp \
                        $(SRC_DIR)/Parser.cpp

GAMEOFLIFE_TEST_SOURCES = $(TEST_DIR)/GameOfLifeTests.cpp \
                          $(SRC_DIR)/GameOfLife.cpp \
                          $(SRC_DIR)/Universe.cpp \
                          $(SRC_DIR)/Parser.cpp \
                          $(SRC_DIR)/Command.cpp

# Заголовочные файлы для зависимостей
HEADERS = $(SRC_DIR)/GameOfLife.h \
          $(SRC_DIR)/Universe.h \
          $(SRC_DIR)/Parser.h \
          $(SRC_DIR)/GameConfig.h \
          $(SRC_DIR)/Command.h

# Цели по умолчанию
all: gameoflife universe_tests gameoflife_tests

# Основная программа
gameoflife: $(MAIN_SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $(MAIN_SOURCES)

# Тесты для Universe
universe_tests: $(UNIVERSE_TEST_SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $(UNIVERSE_TEST_SOURCES) $(TESTFLAGS)

# Тесты для GameOfLife
gameoflife_tests: $(GAMEOFLIFE_TEST_SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $(GAMEOFLIFE_TEST_SOURCES) $(TESTFLAGS)

# Запуск всех тестов
test: universe_tests gameoflife_tests
	@echo "=== Running Universe tests ==="
	./universe_tests
	@echo "=== Running GameOfLife tests ==="
	./gameoflife_tests

# Очистка
clean:
	rm -f gameoflife universe_tests gameoflife_tests *.life *.o

# Запуск программы
run: gameoflife
	./gameoflife

# Запуск в офлайн-режиме с примером
run_offline: gameoflife
	./gameoflife --input sample.life --iterations 10 --output result.life

# Создание примера файла
sample.life:
	@echo "#Life 1.06" > sample.life
	@echo "#N Glider" >> sample.life
	@echo "#R B3/S23" >> sample.life
	@echo "0 0" >> sample.life
	@echo "1 1" >> sample.life
	@echo "2 1" >> sample.life
	@echo "0 2" >> sample.life
	@echo "1 2" >> sample.life

# Отладочная сборка с информацией
debug: CXXFLAGS += -g -DDEBUG
debug: gameoflife

# Быстрая проверка компиляции без линковки
check:
	$(CXX) $(CXXFLAGS) -c $(MAIN_SOURCES)

# Информация о проекте
info:
	@echo "=== Project Structure ==="
	@echo "Source files:"
	@ls -la $(SRC_DIR)/*.cpp $(SRC_DIR)/*.h
	@echo ""
	@echo "Test files:"
	@ls -la $(TEST_DIR)/*.cpp

.PHONY: all test clean run run_offline debug check info
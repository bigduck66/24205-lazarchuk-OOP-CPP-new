CXX = g++
CXXFLAGS = -std=c++17 -I./src
TESTFLAGS = -lgtest -lgtest_main -lpthread

all: gameoflife universe_tests gameoflife_tests

gameoflife: src/main.cpp src/GameOfLife.cpp src/Universe.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

universe_tests: tests/UniverseTests.cpp src/Universe.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(TESTFLAGS)

gameoflife_tests: tests/GameOfLifeTests.cpp src/GameOfLife.cpp src/Universe.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(TESTFLAGS)

test: universe_tests gameoflife_tests
	./universe_tests
	./gameoflife_tests

clean:
	rm -f gameoflife universe_tests gameoflife_tests *.life

run:
	./gameoflife
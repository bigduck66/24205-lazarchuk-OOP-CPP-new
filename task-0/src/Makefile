CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -I.
TARGET = word_count

SOURCES = main.cpp file_reader.cpp text_processor.cpp word_analyzer.cpp csv_writer.cpp
OBJECTS = $(SOURCES:.cpp=.o)

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) *.o

test:
	$(MAKE) -C ../tests test

# Быстрая компиляция без создания .o файлов
quick:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)
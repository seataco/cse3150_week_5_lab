# Makefile for C++ 2048 game project

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -Wextra

# Target executable
TARGET = solution

# Source files
SOURCES = solution.cpp

# Object files (derived from source files)
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target - remove compiled files
clean:
	rm -f $(OBJECTS) $(TARGET) *.o

# Test target - run pytest on test_game.py
test: $(TARGET)
	pytest test_game.py

# Phony targets (not actual files)
.PHONY: all clean test

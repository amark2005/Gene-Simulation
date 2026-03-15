CXX = g++
CXXFLAGS = -O3 -march=native -flto -Wall -Wextra -std=c++11
LDFLAGS = -flto
LIBS = -lm

TARGET = main
SRC = main.cpp

all: run

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS) $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
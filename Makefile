CXX = g++
CXXFLAGS = -O3
LIBS = -lGL -lm -lpthread -ldl -lrt -lX11

TARGET = main
SRC = main.cpp

all: run

$(TARGET): $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS) $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean

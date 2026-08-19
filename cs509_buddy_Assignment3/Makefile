CXX = g++
CXXFLAGS = -O2 -std=c++17 -Iinclude -Wall
SRC = src/main.cpp src/csr_graph.cpp src/gradient_descent.cpp src/maxflow_mincut.cpp
BIN = build/assignment3

all: $(BIN)

$(BIN): $(SRC)
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(SRC) -o $(BIN)

clean:
	rm -rf build

.PHONY: all clean

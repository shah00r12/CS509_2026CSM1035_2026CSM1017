CXX = g++
CXXFLAGS = -Iheaders

TARGET = main.exe

SOURCES = driver/main.cpp \
          source/kmeans.cpp \
          source/fastmap.cpp

OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

driver/main.o: driver/main.cpp
	$(CXX) $(CXXFLAGS) -c driver/main.cpp -o driver/main.o

source/kmeans.o: source/kmeans.cpp
	$(CXX) $(CXXFLAGS) -c source/kmeans.cpp -o source/kmeans.o

source/fastmap.o: source/fastmap.cpp
	$(CXX) $(CXXFLAGS) -c source/fastmap.cpp -o source/fastmap.o

clean:
	del /Q driver\main.o source\kmeans.o source\fastmap.o $(TARGET) 2>nul
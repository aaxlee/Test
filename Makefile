CXX = g++
CXXFLAGS = -I/C:/libcurl/include -I/C:/jsoncpp/include -std=c++17
LDFLAGS = -L/C:/libcurl/lib -L/C:/jsoncpp/lib -lcurl -ljsoncpp

TARGET = main.exe
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
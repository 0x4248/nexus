
CXX = g++
CXXFLAGS = -Wall -O2 -std=c++17
LDFLAGS = -lcrypto
SRCS = src/main.cpp src/crypto.cpp src/image.cpp src/render.cpp

OBJS = $(SRCS:src/%.cpp=build/%.o)
TARGET = randomart

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o build/$(TARGET) $(LDFLAGS)

build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)

rebuild: clean all

.PHONY: all clean rebuild

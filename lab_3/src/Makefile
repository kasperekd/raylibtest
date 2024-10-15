CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++11

TARGET = build/correlation
SRCS = main.cpp
OBJS = $(patsubst %.cpp, build/%.o, $(SRCS))

all: build_dir $(TARGET)

build_dir:
	@mkdir -p build

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

build/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build $(OBJS)

distclean: clean
	rm -rf build

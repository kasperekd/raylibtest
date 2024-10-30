CXX = gcc
CXXFLAGS = -Wall -Wextra -Werror -Wno-unused-parameter -std=c23

TARGET = build/main
SRCS = main.c
OBJS = $(patsubst %.c, build/%.o, $(SRCS))

all: build_dir $(TARGET)

build_dir:
	@mkdir -p build

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

build/%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build $(OBJS)

distclean: clean
	rm -rf build

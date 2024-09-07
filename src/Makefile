CC = gcc
BUILD_DIR = ../build
SRC_DIR = .
INCLUDE_DIR = ../lib/include
LIB_DIR = ../lib/lib
LIBS = $(BUILD_DIR)/libmyRay.a $(BUILD_DIR)/libmyCalc.a

CFLAGS = -std=c11 -Wall -Wextra -Werror
IFLAGS = -I$(SRC_DIR)/include -I$(INCLUDE_DIR)
LDFLAGS = -L$(LIB_DIR) -L$(BUILD_DIR) -Wl,-rpath=./lib/lib -Wl,-rpath=$(BUILD_DIR) -lmyRay -lraylib -lm -lpthread -ldl -lrt

SRC = main.c
OBJ = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC))

all: $(BUILD_DIR)/main

$(BUILD_DIR)/main: $(OBJ) $(LIBS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(IFLAGS)

.PHONY: all clean

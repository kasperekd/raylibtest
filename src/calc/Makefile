CC = gcc
AR = ar

BUILD_DIR = ../../build
SRC_DIR = .
INCLUDE_DIR_1 = ../../lib/include
INCLUDE_DIR_2 = ../include

CFLAGS = -std=c11 -Wall -Wextra -Werror
IFLAGS = -I$(INCLUDE_DIR_1) -I$(INCLUDE_DIR_2) -c

ARFLAGS = rcs

SRC = myCalc.c
OBJ = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC))
LIB = $(BUILD_DIR)/libmyClac.a

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) $< -o $@

.PHONY: all clean



# CC = gcc
# AR = ar
# CFLAGS = -I../include -I../../lib/include -c
# ARFLAGS = rcs
# SRC = myRay.c
# OBJ = $(SRC:.c=.o)
# LIB = libmyRay.a
# BUILD_DIR = ../../build

# all: $(BUILD_DIR)/$(LIB)

# $(BUILD_DIR)/$(LIB): $(OBJ)
# 	$(AR) $(ARFLAGS) $@ $^

# %.o: %.c
# 	$(CC) $(CFLAGS) $< -o $@

# clean:
# 	rm -f $(OBJ)

# $(BUILDDIR):
# 	mkdir -p $(BUILDDIR)

# .PHONY: all clean
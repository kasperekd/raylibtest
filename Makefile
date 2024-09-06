VISUAL_DIR = src/visual
MAIN_DIR = src
BUILD_DIR = build

all: visual main

all:
	$(shell mkdir -p $(BUILD_DIR))
	$(MAKE) -C $(VISUAL_DIR)
	$(MAKE) -C $(MAIN_DIR)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean visual main
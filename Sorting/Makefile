CC := cc
CFLAGS := -std=c11 -Wall -Wextra -O2 -IC/include
BIN_DIR := bin
C_DIR := C
DEMO := $(BIN_DIR)/sorting_demo
DEMO_SOURCES := $(C_DIR)/src/main.c $(C_DIR)/src/algorithms.c $(C_DIR)/src/utils.c

.PHONY: all clean run demo

all: demo

demo: $(DEMO)

$(DEMO): $(DEMO_SOURCES) $(C_DIR)/include/sorting.h | $(BIN_DIR)
	$(CC) $(CFLAGS) $(DEMO_SOURCES) -o $(DEMO)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

run: demo
	(cd $(C_DIR) && ../$(DEMO) all)

clean:
	rm -rf $(BIN_DIR)

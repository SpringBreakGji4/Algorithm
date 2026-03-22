CC := cc
CFLAGS := -std=c11 -Wall -Wextra -O2 -Iinclude
BIN_DIR := bin
TARGET := $(BIN_DIR)/trie_demo
SOURCES := src/main.c src/trie.c

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SOURCES) include/trie.h | $(BIN_DIR)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BIN_DIR)

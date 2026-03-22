CC := cc
CFLAGS := -std=c11 -Wall -Wextra -O2 -Iinclude
BIN_DIR := bin
TRIE_TARGET := $(BIN_DIR)/trie_demo
BST_TARGET := $(BIN_DIR)/bst_demo
TRIE_SOURCES := src/main.c src/trie.c
BST_SOURCES := src/bst_main.c src/bst.c

.PHONY: all clean run-trie run-bst

all: $(TRIE_TARGET) $(BST_TARGET)

$(TRIE_TARGET): $(TRIE_SOURCES) include/trie.h | $(BIN_DIR)
	$(CC) $(CFLAGS) $(TRIE_SOURCES) -o $(TRIE_TARGET)

$(BST_TARGET): $(BST_SOURCES) include/bst.h | $(BIN_DIR)
	$(CC) $(CFLAGS) $(BST_SOURCES) -o $(BST_TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

run-trie: $(TRIE_TARGET)
	./$(TRIE_TARGET)

run-bst: $(BST_TARGET)
	./$(BST_TARGET)

clean:
	rm -rf $(BIN_DIR)

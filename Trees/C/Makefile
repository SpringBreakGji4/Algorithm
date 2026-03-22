CC := cc
CFLAGS := -std=c11 -Wall -Wextra -O2 -Iinclude
BIN_DIR := bin
TRIE_TARGET := $(BIN_DIR)/trie_demo
BST_TARGET := $(BIN_DIR)/bst_demo
HEAP_TARGET := $(BIN_DIR)/heap_demo
AVL_TARGET := $(BIN_DIR)/avl_demo
TRIE_SOURCES := src/main.c src/trie.c
BST_SOURCES := src/bst_main.c src/bst.c
HEAP_SOURCES := src/heap_main.c src/heap.c
AVL_SOURCES := src/avl_main.c src/avl.c

.PHONY: all clean run-trie run-bst run-heap run-avl

all: $(TRIE_TARGET) $(BST_TARGET) $(HEAP_TARGET) $(AVL_TARGET)

$(TRIE_TARGET): $(TRIE_SOURCES) include/trie.h | $(BIN_DIR)
	$(CC) $(CFLAGS) $(TRIE_SOURCES) -o $(TRIE_TARGET)

$(BST_TARGET): $(BST_SOURCES) include/bst.h | $(BIN_DIR)
	$(CC) $(CFLAGS) $(BST_SOURCES) -o $(BST_TARGET)

$(HEAP_TARGET): $(HEAP_SOURCES) include/heap.h | $(BIN_DIR)
	$(CC) $(CFLAGS) $(HEAP_SOURCES) -o $(HEAP_TARGET)

$(AVL_TARGET): $(AVL_SOURCES) include/avl.h | $(BIN_DIR)
	$(CC) $(CFLAGS) $(AVL_SOURCES) -o $(AVL_TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

run-trie: $(TRIE_TARGET)
	./$(TRIE_TARGET)

run-bst: $(BST_TARGET)
	./$(BST_TARGET)

run-heap: $(HEAP_TARGET)
	./$(HEAP_TARGET)

run-avl: $(AVL_TARGET)
	./$(AVL_TARGET)

clean:
	rm -rf $(BIN_DIR)

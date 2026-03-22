#include "trie.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int char_index(char c){
    if(c >= 'A' && c <= 'Z'){
        c = (char)(c - 'A' + 'a');
    }
    if(c < 'a' || c > 'z'){
        return -1;
    }
    return c - 'a';
}

TrieNode *trie_create_node(void){
    TrieNode *node = (TrieNode *)calloc(1, sizeof(TrieNode));
    return node;
}

void trie_insert(TrieNode *root, const char *word){
    TrieNode *current = root;
    for(size_t i = 0; word[i] != '\0'; i++){
        int index = char_index(word[i]);
        if(index < 0){
            continue;
        }
        if(current->children[index] == NULL){
            current->children[index] = trie_create_node();
        }
        current = current->children[index];
    }
    current->is_end_of_word = 1;
}

static TrieNode *trie_find_node(TrieNode *root, const char *text){
    TrieNode *current = root;
    for(size_t i = 0; text[i] != '\0'; i++){
        int index = char_index(text[i]);
        if(index < 0){
            continue;
        }
        if(current->children[index] == NULL){
            return NULL;
        }
        current = current->children[index];
    }
    return current;
}

int trie_search(TrieNode *root, const char *word){
    TrieNode *node = trie_find_node(root, word);
    return node != NULL && node->is_end_of_word;
}

int trie_starts_with(TrieNode *root, const char *prefix){
    return trie_find_node(root, prefix) != NULL;
}

static int has_children(TrieNode *node){
    for(int i = 0; i < ALPHABET_SIZE; i++){
        if(node->children[i] != NULL){
            return 1;
        }
    }
    return 0;
}

static int trie_delete_recursive(TrieNode *node, const char *word, size_t depth, int *deleted){
    if(node == NULL){
        return 0;
    }

    if(word[depth] == '\0'){
        if(!node->is_end_of_word){
            return 0;
        }
        node->is_end_of_word = 0;
        *deleted = 1;
        return !has_children(node);
    }

    int index = char_index(word[depth]);
    if(index < 0){
        return trie_delete_recursive(node, word, depth + 1, deleted);
    }
    if(node->children[index] == NULL){
        return 0;
    }

    if(trie_delete_recursive(node->children[index], word, depth + 1, deleted)){
        trie_free(node->children[index]);
        node->children[index] = NULL;
        return !node->is_end_of_word && !has_children(node);
    }

    return 0;
}

int trie_delete(TrieNode *root, const char *word){
    int deleted = 0;
    trie_delete_recursive(root, word, 0, &deleted);
    return deleted;
}

static int trie_count_words(TrieNode *node){
    if(node == NULL){
        return 0;
    }
    int total = node->is_end_of_word;
    for(int i = 0; i < ALPHABET_SIZE; i++){
        total += trie_count_words(node->children[i]);
    }
    return total;
}

int trie_count_prefix(TrieNode *root, const char *prefix){
    TrieNode *node = trie_find_node(root, prefix);
    if(node == NULL){
        return 0;
    }
    return trie_count_words(node);
}

static void trie_collect_words(TrieNode *node, char *buffer, int depth, int *printed){
    if(node == NULL){
        return;
    }

    if(node->is_end_of_word){
        buffer[depth] = '\0';
        if(*printed){
            printf(", ");
        }
        printf("%s", buffer);
        *printed = 1;
    }

    for(int i = 0; i < ALPHABET_SIZE; i++){
        if(node->children[i] != NULL){
            buffer[depth] = (char)('a' + i);
            trie_collect_words(node->children[i], buffer, depth + 1, printed);
        }
    }
}

void trie_autocomplete(TrieNode *root, const char *prefix){
    TrieNode *node = trie_find_node(root, prefix);
    char buffer[256];
    int printed = 0;
    size_t length = strlen(prefix);

    if(node == NULL){
        printf("(none)");
        return;
    }

    strncpy(buffer, prefix, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    trie_collect_words(node, buffer, (int)length, &printed);
    if(!printed){
        printf("(none)");
    }
}

void trie_free(TrieNode *root){
    if(root == NULL){
        return;
    }
    for(int i = 0; i < ALPHABET_SIZE; i++){
        trie_free(root->children[i]);
    }
    free(root);
}

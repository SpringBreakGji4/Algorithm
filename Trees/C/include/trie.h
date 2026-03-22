#ifndef TRIE_H
#define TRIE_H

#define ALPHABET_SIZE 26

typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int is_end_of_word;
} TrieNode;

TrieNode *trie_create_node(void);
void trie_insert(TrieNode *root, const char *word);
int trie_search(TrieNode *root, const char *word);
int trie_starts_with(TrieNode *root, const char *prefix);
int trie_delete(TrieNode *root, const char *word);
int trie_count_prefix(TrieNode *root, const char *prefix);
void trie_autocomplete(TrieNode *root, const char *prefix);
void trie_free(TrieNode *root);

#endif

#include "trie.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_QUERY_FILE "../test_case/trie_queries.txt"
#define MAX_LINE_LENGTH 512

static void trim_newline(char *text){
    text[strcspn(text, "\r\n")] = '\0';
}

int main(int argc, char **argv){
    const char *query_file = DEFAULT_QUERY_FILE;
    char line[MAX_LINE_LENGTH];

    if(argc > 1){
        query_file = argv[1];
    }

    FILE *file = fopen(query_file, "r");
    if(file == NULL){
        fprintf(stderr, "Query file not found: %s\n", query_file);
        return 1;
    }

    TrieNode *root = trie_create_node();
    if(root == NULL){
        fclose(file);
        fprintf(stderr, "Failed to create trie root\n");
        return 1;
    }

    printf("Trie / Prefix Tree Demo (C)\n");
    printf("============================\n");

    while(fgets(line, sizeof(line), file) != NULL){
        trim_newline(line);
        if(line[0] == '\0' || line[0] == '#'){
            continue;
        }

        char *command = strtok(line, " ");
        char *argument = strtok(NULL, "");
        if(argument == NULL){
            argument = "";
        }

        if(strcmp(command, "insert") == 0){
            trie_insert(root, argument);
            printf("insert %s -> OK\n", argument);
        }
        else if(strcmp(command, "search") == 0){
            printf("search %s -> %s\n", argument, trie_search(root, argument) ? "FOUND" : "NOT FOUND");
        }
        else if(strcmp(command, "prefix") == 0){
            printf("prefix %s -> %s\n", argument, trie_starts_with(root, argument) ? "YES" : "NO");
        }
        else if(strcmp(command, "delete") == 0){
            printf("delete %s -> %s\n", argument, trie_delete(root, argument) ? "DELETED" : "NOT FOUND");
        }
        else if(strcmp(command, "autocomplete") == 0){
            printf("autocomplete %s -> ", argument);
            trie_autocomplete(root, argument);
            printf("\n");
        }
        else if(strcmp(command, "countprefix") == 0){
            printf("countprefix %s -> %d\n", argument, trie_count_prefix(root, argument));
        }
        else{
            printf("unknown command: %s %s\n", command, argument);
        }
    }

    trie_free(root);
    fclose(file);
    return 0;
}

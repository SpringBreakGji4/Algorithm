#include "avl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_QUERY_FILE "../test_case/avl_queries.txt"
#define MAX_LINE_LENGTH 512

static void trim_newline(char *text){
    text[strcspn(text, "\r\n")] = '\0';
}

int main(int argc, char **argv){
    const char *query_file = DEFAULT_QUERY_FILE;
    char line[MAX_LINE_LENGTH];
    AVLNode *root = NULL;

    if(argc > 1){
        query_file = argv[1];
    }

    FILE *file = fopen(query_file, "r");
    if(file == NULL){
        fprintf(stderr, "Query file not found: %s\n", query_file);
        return 1;
    }

    printf("AVL Tree Demo (C)\n");
    printf("=================\n");

    while(fgets(line, sizeof(line), file) != NULL){
        trim_newline(line);
        if(line[0] == '\0' || line[0] == '#'){
            continue;
        }

        char *command = strtok(line, " ");
        char *argument = strtok(NULL, "");

        if(strcmp(command, "insert") == 0){
            root = avl_insert(root, atoi(argument));
            printf("insert %s -> OK\n", argument);
        }
        else if(strcmp(command, "delete") == 0){
            int value = atoi(argument);
            int existed = avl_search(root, value);
            root = avl_delete(root, value);
            printf("delete %s -> %s\n", argument, existed ? "DELETED" : "NOT FOUND");
        }
        else if(strcmp(command, "search") == 0){
            printf("search %s -> %s\n", argument, avl_search(root, atoi(argument)) ? "FOUND" : "NOT FOUND");
        }
        else if(strcmp(command, "inorder") == 0){
            printf("inorder -> ");
            avl_inorder(root);
            printf("\n");
        }
        else if(strcmp(command, "preorder") == 0){
            printf("preorder -> ");
            avl_preorder(root);
            printf("\n");
        }
        else if(strcmp(command, "height") == 0){
            printf("height -> %d\n", avl_height_value(root));
        }
        else{
            printf("unknown command: %s\n", command);
        }
    }

    avl_free(root);
    fclose(file);
    return 0;
}

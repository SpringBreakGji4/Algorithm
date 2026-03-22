#include "bst.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_QUERY_FILE "../test_case/bst_queries.txt"
#define MAX_LINE_LENGTH 512

static void trim_newline(char *text){
    text[strcspn(text, "\r\n")] = '\0';
}

int main(int argc, char **argv){
    const char *query_file = DEFAULT_QUERY_FILE;
    char line[MAX_LINE_LENGTH];
    BSTNode *root = NULL;

    if(argc > 1){
        query_file = argv[1];
    }

    FILE *file = fopen(query_file, "r");
    if(file == NULL){
        fprintf(stderr, "Query file not found: %s\n", query_file);
        return 1;
    }

    printf("Binary Search Tree Demo (C)\n");
    printf("===========================\n");

    while(fgets(line, sizeof(line), file) != NULL){
        trim_newline(line);
        if(line[0] == '\0' || line[0] == '#'){
            continue;
        }

        char *command = strtok(line, " ");
        char *argument = strtok(NULL, "");

        if(strcmp(command, "insert") == 0){
            root = bst_insert(root, atoi(argument));
            printf("insert %s -> OK\n", argument);
        }
        else if(strcmp(command, "search") == 0){
            printf("search %s -> %s\n", argument, bst_search(root, atoi(argument)) ? "FOUND" : "NOT FOUND");
        }
        else if(strcmp(command, "delete") == 0){
            int value = atoi(argument);
            int existed = bst_search(root, value);
            root = bst_delete(root, value);
            printf("delete %s -> %s\n", argument, existed ? "DELETED" : "NOT FOUND");
        }
        else if(strcmp(command, "min") == 0){
            BSTNode *node = bst_find_min(root);
            printf("min -> %s", node == NULL ? "(empty)" : "");
            if(node != NULL){
                printf("%d", node->value);
            }
            printf("\n");
        }
        else if(strcmp(command, "max") == 0){
            BSTNode *node = bst_find_max(root);
            printf("max -> %s", node == NULL ? "(empty)" : "");
            if(node != NULL){
                printf("%d", node->value);
            }
            printf("\n");
        }
        else if(strcmp(command, "inorder") == 0){
            printf("inorder -> ");
            bst_inorder(root);
            printf("\n");
        }
        else if(strcmp(command, "preorder") == 0){
            printf("preorder -> ");
            bst_preorder(root);
            printf("\n");
        }
        else if(strcmp(command, "postorder") == 0){
            printf("postorder -> ");
            bst_postorder(root);
            printf("\n");
        }
        else if(strcmp(command, "height") == 0){
            printf("height -> %d\n", bst_height(root));
        }
        else{
            printf("unknown command: %s\n", command);
        }
    }

    bst_free(root);
    fclose(file);
    return 0;
}

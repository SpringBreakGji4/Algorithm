#include "segment_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_QUERY_FILE "../test_case/segment_tree_queries.txt"
#define MAX_LINE_LENGTH 512
#define MAX_VALUES 128

static void trim_newline(char *text){
    text[strcspn(text, "\r\n")] = '\0';
}

int main(int argc, char **argv){
    const char *query_file = DEFAULT_QUERY_FILE;
    char line[MAX_LINE_LENGTH];
    SegmentTree segment_tree;
    segment_tree_init(&segment_tree);

    if(argc > 1){
        query_file = argv[1];
    }

    FILE *file = fopen(query_file, "r");
    if(file == NULL){
        fprintf(stderr, "Query file not found: %s\n", query_file);
        return 1;
    }

    printf("Segment Tree Demo (C)\n");
    printf("=====================\n");

    while(fgets(line, sizeof(line), file) != NULL){
        trim_newline(line);
        if(line[0] == '\0' || line[0] == '#'){
            continue;
        }

        char *command = strtok(line, " ");
        if(strcmp(command, "build") == 0){
            int values[MAX_VALUES];
            int count = 0;
            char *token = strtok(NULL, " ");
            while(token != NULL && count < MAX_VALUES){
                values[count++] = atoi(token);
                token = strtok(NULL, " ");
            }
            segment_tree_build(&segment_tree, values, count);
            printf("build -> ");
            segment_tree_print_values(&segment_tree);
            printf("\n");
        }
        else if(strcmp(command, "print") == 0){
            printf("values -> ");
            segment_tree_print_values(&segment_tree);
            printf("\n");
        }
        else if(strcmp(command, "query") == 0){
            int left = atoi(strtok(NULL, " "));
            int right = atoi(strtok(NULL, " "));
            printf("query %d %d -> %d\n", left, right, segment_tree_query(&segment_tree, left, right));
        }
        else if(strcmp(command, "update") == 0){
            int index = atoi(strtok(NULL, " "));
            int delta = atoi(strtok(NULL, " "));
            segment_tree_update(&segment_tree, index, delta);
            printf("update %d %d -> OK\n", index, delta);
        }
        else if(strcmp(command, "set") == 0){
            int index = atoi(strtok(NULL, " "));
            int value = atoi(strtok(NULL, " "));
            segment_tree_set(&segment_tree, index, value);
            printf("set %d %d -> OK\n", index, value);
        }
        else{
            printf("unknown command: %s\n", command);
        }
    }

    segment_tree_free(&segment_tree);
    fclose(file);
    return 0;
}

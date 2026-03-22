#include "fenwick.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_QUERY_FILE "../test_case/fenwick_queries.txt"
#define MAX_LINE_LENGTH 512
#define MAX_VALUES 128

static void trim_newline(char *text){
    text[strcspn(text, "\r\n")] = '\0';
}

int main(int argc, char **argv){
    const char *query_file = DEFAULT_QUERY_FILE;
    char line[MAX_LINE_LENGTH];
    FenwickTree fenwick;
    fenwick_init(&fenwick);

    if(argc > 1){
        query_file = argv[1];
    }

    FILE *file = fopen(query_file, "r");
    if(file == NULL){
        fprintf(stderr, "Query file not found: %s\n", query_file);
        return 1;
    }

    printf("Fenwick Tree Demo (C)\n");
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
            fenwick_build(&fenwick, values, count);
            printf("build -> ");
            fenwick_print_values(&fenwick);
            printf("\n");
        }
        else if(strcmp(command, "print") == 0){
            printf("values -> ");
            fenwick_print_values(&fenwick);
            printf("\n");
        }
        else if(strcmp(command, "prefix") == 0){
            int index = atoi(strtok(NULL, " "));
            printf("prefix %d -> %d\n", index, fenwick_prefix_sum(&fenwick, index));
        }
        else if(strcmp(command, "range") == 0){
            int left = atoi(strtok(NULL, " "));
            int right = atoi(strtok(NULL, " "));
            printf("range %d %d -> %d\n", left, right, fenwick_range_sum(&fenwick, left, right));
        }
        else if(strcmp(command, "update") == 0){
            int index = atoi(strtok(NULL, " "));
            int delta = atoi(strtok(NULL, " "));
            fenwick_update(&fenwick, index, delta);
            printf("update %d %d -> OK\n", index, delta);
        }
        else if(strcmp(command, "set") == 0){
            int index = atoi(strtok(NULL, " "));
            int value = atoi(strtok(NULL, " "));
            fenwick_set(&fenwick, index, value);
            printf("set %d %d -> OK\n", index, value);
        }
        else{
            printf("unknown command: %s\n", command);
        }
    }

    fenwick_free(&fenwick);
    fclose(file);
    return 0;
}

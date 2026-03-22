#include "heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_QUERY_FILE "../test_case/heap_queries.txt"
#define MAX_LINE_LENGTH 512
#define MAX_VALUES 128

static void trim_newline(char *text){
    text[strcspn(text, "\r\n")] = '\0';
}

int main(int argc, char **argv){
    const char *query_file = DEFAULT_QUERY_FILE;
    char line[MAX_LINE_LENGTH];
    MinHeap heap;
    heap_init(&heap);

    if(argc > 1){
        query_file = argv[1];
    }

    FILE *file = fopen(query_file, "r");
    if(file == NULL){
        fprintf(stderr, "Query file not found: %s\n", query_file);
        heap_free(&heap);
        return 1;
    }

    printf("Heap / Priority Queue Demo (C)\n");
    printf("================================\n");

    while(fgets(line, sizeof(line), file) != NULL){
        trim_newline(line);
        if(line[0] == '\0' || line[0] == '#'){
            continue;
        }

        char *command = strtok(line, " ");
        if(strcmp(command, "push") == 0){
            char *arg = strtok(NULL, " ");
            int value = atoi(arg);
            heap_push(&heap, value);
            printf("push %d -> OK\n", value);
        }
        else if(strcmp(command, "pop") == 0){
            int value;
            if(heap_pop(&heap, &value)){
                printf("pop -> %d\n", value);
            }
            else{
                printf("pop -> (empty)\n");
            }
        }
        else if(strcmp(command, "peek") == 0){
            int value;
            if(heap_peek(&heap, &value)){
                printf("peek -> %d\n", value);
            }
            else{
                printf("peek -> (empty)\n");
            }
        }
        else if(strcmp(command, "size") == 0){
            printf("size -> %d\n", heap.size);
        }
        else if(strcmp(command, "print") == 0){
            printf("heap -> ");
            heap_print(&heap);
            printf("\n");
        }
        else if(strcmp(command, "heapify") == 0){
            int values[MAX_VALUES];
            int count = 0;
            char *token = strtok(NULL, " ");
            while(token != NULL && count < MAX_VALUES){
                values[count++] = atoi(token);
                token = strtok(NULL, " ");
            }
            heap_heapify(&heap, values, count);
            printf("heapify -> ");
            heap_print(&heap);
            printf("\n");
        }
        else{
            printf("unknown command: %s\n", command);
        }
    }

    heap_free(&heap);
    fclose(file);
    return 0;
}

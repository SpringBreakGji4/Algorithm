#include "sorting.h"

#include <stdio.h>
#include <time.h>

int load_test_case(const char *path, int *buffer, int max_size){
    FILE *file = fopen(path, "r");
    int size = 0;

    if(file == NULL){
        return -1;
    }

    while(size < max_size && fscanf(file, "%d", &buffer[size]) == 1){
        size++;
    }

    fclose(file);
    return size;
}

int check_array_sorted(const int *array, int size){
    for(int i = 1; i < size; i++){
        if(array[i] < array[i - 1]){
            return 0;
        }
    }
    return 1;
}

long long current_timestamp_us(void){
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return (long long)time.tv_sec * 1000000LL + time.tv_nsec / 1000LL;
}

void print_array(const int *array, int size){
    for(int i = 0; i < size; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

#include "fenwick.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void fenwick_add(FenwickTree *fenwick, int index, int delta){
    while(index <= fenwick->size){
        fenwick->tree[index] += delta;
        index += index & -index;
    }
}

void fenwick_init(FenwickTree *fenwick){
    fenwick->tree = NULL;
    fenwick->values = NULL;
    fenwick->size = 0;
}

void fenwick_build(FenwickTree *fenwick, const int *values, int count){
    free(fenwick->tree);
    free(fenwick->values);

    fenwick->size = count;
    fenwick->tree = (int *)calloc((size_t)count + 1, sizeof(int));
    fenwick->values = (int *)malloc(sizeof(int) * count);

    if((count > 0 && fenwick->tree == NULL) || (count > 0 && fenwick->values == NULL)){
        fprintf(stderr, "Failed to allocate Fenwick tree\n");
        exit(1);
    }

    for(int i = 0; i < count; i++){
        fenwick->values[i] = values[i];
        fenwick_add(fenwick, i + 1, values[i]);
    }
}

void fenwick_update(FenwickTree *fenwick, int index, int delta){
    fenwick->values[index - 1] += delta;
    fenwick_add(fenwick, index, delta);
}

void fenwick_set(FenwickTree *fenwick, int index, int value){
    int current = fenwick->values[index - 1];
    fenwick_update(fenwick, index, value - current);
}

int fenwick_prefix_sum(FenwickTree *fenwick, int index){
    int sum = 0;
    while(index > 0){
        sum += fenwick->tree[index];
        index -= index & -index;
    }
    return sum;
}

int fenwick_range_sum(FenwickTree *fenwick, int left, int right){
    return fenwick_prefix_sum(fenwick, right) - fenwick_prefix_sum(fenwick, left - 1);
}

void fenwick_print_values(FenwickTree *fenwick){
    if(fenwick->size == 0){
        printf("(empty)");
        return;
    }
    for(int i = 0; i < fenwick->size; i++){
        if(i > 0){
            printf(" ");
        }
        printf("%d", fenwick->values[i]);
    }
}

void fenwick_free(FenwickTree *fenwick){
    free(fenwick->tree);
    free(fenwick->values);
    fenwick->tree = NULL;
    fenwick->values = NULL;
    fenwick->size = 0;
}

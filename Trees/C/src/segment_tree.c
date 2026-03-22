#include "segment_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void build_recursive(SegmentTree *segment_tree, int node, int left, int right){
    if(left == right){
        segment_tree->tree[node] = segment_tree->values[left];
        return;
    }
    int mid = (left + right) / 2;
    build_recursive(segment_tree, node * 2, left, mid);
    build_recursive(segment_tree, node * 2 + 1, mid + 1, right);
    segment_tree->tree[node] = segment_tree->tree[node * 2] + segment_tree->tree[node * 2 + 1];
}

static int query_recursive(SegmentTree *segment_tree, int node, int left, int right, int query_left, int query_right){
    if(query_right < left || right < query_left){
        return 0;
    }
    if(query_left <= left && right <= query_right){
        return segment_tree->tree[node];
    }
    int mid = (left + right) / 2;
    return query_recursive(segment_tree, node * 2, left, mid, query_left, query_right) +
           query_recursive(segment_tree, node * 2 + 1, mid + 1, right, query_left, query_right);
}

static void update_recursive(SegmentTree *segment_tree, int node, int left, int right, int index, int value){
    if(left == right){
        segment_tree->tree[node] = value;
        return;
    }
    int mid = (left + right) / 2;
    if(index <= mid){
        update_recursive(segment_tree, node * 2, left, mid, index, value);
    }
    else{
        update_recursive(segment_tree, node * 2 + 1, mid + 1, right, index, value);
    }
    segment_tree->tree[node] = segment_tree->tree[node * 2] + segment_tree->tree[node * 2 + 1];
}

void segment_tree_init(SegmentTree *segment_tree){
    segment_tree->tree = NULL;
    segment_tree->values = NULL;
    segment_tree->size = 0;
}

void segment_tree_build(SegmentTree *segment_tree, const int *values, int count){
    free(segment_tree->tree);
    free(segment_tree->values);

    segment_tree->size = count;
    if(count == 0){
        segment_tree->tree = NULL;
        segment_tree->values = NULL;
        return;
    }

    segment_tree->values = (int *)malloc(sizeof(int) * count);
    segment_tree->tree = (int *)calloc((size_t)(4 * count), sizeof(int));
    if(segment_tree->values == NULL || segment_tree->tree == NULL){
        fprintf(stderr, "Failed to allocate segment tree\n");
        exit(1);
    }

    memcpy(segment_tree->values, values, sizeof(int) * count);
    build_recursive(segment_tree, 1, 0, count - 1);
}

int segment_tree_query(SegmentTree *segment_tree, int left, int right){
    if(segment_tree->size == 0){
        return 0;
    }
    return query_recursive(segment_tree, 1, 0, segment_tree->size - 1, left - 1, right - 1);
}

void segment_tree_update(SegmentTree *segment_tree, int index, int delta){
    segment_tree->values[index - 1] += delta;
    update_recursive(segment_tree, 1, 0, segment_tree->size - 1, index - 1, segment_tree->values[index - 1]);
}

void segment_tree_set(SegmentTree *segment_tree, int index, int value){
    segment_tree->values[index - 1] = value;
    update_recursive(segment_tree, 1, 0, segment_tree->size - 1, index - 1, value);
}

void segment_tree_print_values(SegmentTree *segment_tree){
    if(segment_tree->size == 0){
        printf("(empty)");
        return;
    }
    for(int i = 0; i < segment_tree->size; i++){
        if(i > 0){
            printf(" ");
        }
        printf("%d", segment_tree->values[i]);
    }
}

void segment_tree_free(SegmentTree *segment_tree){
    free(segment_tree->tree);
    free(segment_tree->values);
    segment_tree->tree = NULL;
    segment_tree->values = NULL;
    segment_tree->size = 0;
}

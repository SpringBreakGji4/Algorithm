#include "heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

static void ensure_capacity(MinHeap *heap, int required){
    if(heap->capacity >= required){
        return;
    }
    int new_capacity = heap->capacity == 0 ? 8 : heap->capacity * 2;
    while(new_capacity < required){
        new_capacity *= 2;
    }
    int *new_data = (int *)realloc(heap->data, sizeof(int) * new_capacity);
    if(new_data == NULL){
        fprintf(stderr, "Failed to allocate heap memory\n");
        exit(1);
    }
    heap->data = new_data;
    heap->capacity = new_capacity;
}

static void sift_up(MinHeap *heap, int index){
    while(index > 0){
        int parent = (index - 1) / 2;
        if(heap->data[parent] <= heap->data[index]){
            break;
        }
        swap(&heap->data[parent], &heap->data[index]);
        index = parent;
    }
}

static void sift_down(MinHeap *heap, int index){
    while(1){
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if(left < heap->size && heap->data[left] < heap->data[smallest]){
            smallest = left;
        }
        if(right < heap->size && heap->data[right] < heap->data[smallest]){
            smallest = right;
        }
        if(smallest == index){
            break;
        }
        swap(&heap->data[index], &heap->data[smallest]);
        index = smallest;
    }
}

void heap_init(MinHeap *heap){
    heap->data = NULL;
    heap->size = 0;
    heap->capacity = 0;
}

void heap_free(MinHeap *heap){
    free(heap->data);
    heap->data = NULL;
    heap->size = 0;
    heap->capacity = 0;
}

void heap_push(MinHeap *heap, int value){
    ensure_capacity(heap, heap->size + 1);
    heap->data[heap->size] = value;
    sift_up(heap, heap->size);
    heap->size++;
}

int heap_pop(MinHeap *heap, int *value){
    if(heap->size == 0){
        return 0;
    }
    *value = heap->data[0];
    heap->size--;
    if(heap->size > 0){
        heap->data[0] = heap->data[heap->size];
        sift_down(heap, 0);
    }
    return 1;
}

int heap_peek(MinHeap *heap, int *value){
    if(heap->size == 0){
        return 0;
    }
    *value = heap->data[0];
    return 1;
}

void heap_heapify(MinHeap *heap, const int *values, int count){
    ensure_capacity(heap, count);
    memcpy(heap->data, values, sizeof(int) * count);
    heap->size = count;
    for(int i = heap->size / 2 - 1; i >= 0; i--){
        sift_down(heap, i);
    }
}

void heap_print(const MinHeap *heap){
    if(heap->size == 0){
        printf("(empty)");
        return;
    }
    for(int i = 0; i < heap->size; i++){
        if(i > 0){
            printf(" ");
        }
        printf("%d", heap->data[i]);
    }
}

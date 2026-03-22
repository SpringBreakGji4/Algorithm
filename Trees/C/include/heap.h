#ifndef HEAP_H
#define HEAP_H

typedef struct {
    int *data;
    int size;
    int capacity;
} MinHeap;

void heap_init(MinHeap *heap);
void heap_free(MinHeap *heap);
void heap_push(MinHeap *heap, int value);
int heap_pop(MinHeap *heap, int *value);
int heap_peek(MinHeap *heap, int *value);
void heap_heapify(MinHeap *heap, const int *values, int count);
void heap_print(const MinHeap *heap);

#endif

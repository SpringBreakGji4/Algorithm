#include "sorting.h"

#include <stdlib.h>
#include <string.h>

typedef struct Node{
    int data;
    struct Node *next;
} Node;

static void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void bubble_sort(int *array, int size){
    for(int i = 0; i < size - 1; i++){
        int swapped = 0;
        for(int j = 0; j < size - i - 1; j++){
            if(array[j] > array[j + 1]){
                swap(&array[j], &array[j + 1]);
                swapped = 1;
            }
        }
        if(!swapped){
            break;
        }
    }
}

void selection_sort(int *array, int size){
    for(int i = 0; i < size - 1; i++){
        int smallest_index = i;
        for(int j = i + 1; j < size; j++){
            if(array[j] < array[smallest_index]){
                smallest_index = j;
            }
        }
        swap(&array[i], &array[smallest_index]);
    }
}

void insertion_sort(int *array, int size){
    for(int i = 1; i < size; i++){
        int key = array[i];
        int j = i - 1;
        while(j >= 0 && array[j] > key){
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = key;
    }
}

void shell_sort(int *array, int size){
    for(int gap = size / 2; gap > 0; gap /= 2){
        for(int i = gap; i < size; i++){
            int temp = array[i];
            int j = i;
            while(j >= gap && array[j - gap] > temp){
                array[j] = array[j - gap];
                j -= gap;
            }
            array[j] = temp;
        }
    }
}

static void merge_ranges(int *array, int *buffer, int left, int mid, int right){
    int i = left;
    int j = mid + 1;
    int k = left;

    while(i <= mid && j <= right){
        if(array[i] <= array[j]){
            buffer[k++] = array[i++];
        }
        else{
            buffer[k++] = array[j++];
        }
    }

    while(i <= mid){
        buffer[k++] = array[i++];
    }

    while(j <= right){
        buffer[k++] = array[j++];
    }

    for(i = left; i <= right; i++){
        array[i] = buffer[i];
    }
}

static void merge_sort_recursive(int *array, int *buffer, int left, int right){
    if(left >= right){
        return;
    }

    int mid = left + (right - left) / 2;
    merge_sort_recursive(array, buffer, left, mid);
    merge_sort_recursive(array, buffer, mid + 1, right);
    merge_ranges(array, buffer, left, mid, right);
}

void merge_sort(int *array, int size){
    if(size <= 1){
        return;
    }

    int *buffer = (int *)malloc(sizeof(int) * size);
    if(buffer == NULL){
        return;
    }

    merge_sort_recursive(array, buffer, 0, size - 1);
    free(buffer);
}

static void heapify(int *array, int size, int root){
    int largest = root;
    int left = 2 * root + 1;
    int right = 2 * root + 2;

    if(left < size && array[left] > array[largest]){
        largest = left;
    }
    if(right < size && array[right] > array[largest]){
        largest = right;
    }

    if(largest != root){
        swap(&array[root], &array[largest]);
        heapify(array, size, largest);
    }
}

void heap_sort(int *array, int size){
    for(int i = size / 2 - 1; i >= 0; i--){
        heapify(array, size, i);
    }

    for(int i = size - 1; i > 0; i--){
        swap(&array[0], &array[i]);
        heapify(array, i, 0);
    }
}

static int median_of_three(int a, int b, int c){
    if((a <= b && b <= c) || (c <= b && b <= a)){
        return b;
    }
    if((b <= a && a <= c) || (c <= a && a <= b)){
        return a;
    }
    return c;
}

static void quick_sort_recursive(int *array, int left, int right){
    if(left >= right){
        return;
    }

    int pivot = median_of_three(array[left], array[left + (right - left) / 2], array[right]);
    int i = left;
    int j = right;

    while(i <= j){
        while(array[i] < pivot){
            i++;
        }
        while(array[j] > pivot){
            j--;
        }
        if(i <= j){
            swap(&array[i], &array[j]);
            i++;
            j--;
        }
    }

    if(left < j){
        quick_sort_recursive(array, left, j);
    }
    if(i < right){
        quick_sort_recursive(array, i, right);
    }
}

void quick_sort(int *array, int size){
    if(size <= 1){
        return;
    }
    quick_sort_recursive(array, 0, size - 1);
}

static void counting_sort_by_digit(int *array, int size, int exp, int offset){
    int *output = (int *)malloc(sizeof(int) * size);
    int count[10] = {0};

    if(output == NULL){
        return;
    }

    for(int i = 0; i < size; i++){
        count[((array[i] + offset) / exp) % 10]++;
    }

    for(int i = 1; i < 10; i++){
        count[i] += count[i - 1];
    }

    for(int i = size - 1; i >= 0; i--){
        int digit = ((array[i] + offset) / exp) % 10;
        output[count[digit] - 1] = array[i];
        count[digit]--;
    }

    memcpy(array, output, sizeof(int) * size);
    free(output);
}

void radix_sort(int *array, int size){
    if(size <= 1){
        return;
    }

    int min = array[0];
    int max = array[0];

    for(int i = 1; i < size; i++){
        if(array[i] < min){
            min = array[i];
        }
        if(array[i] > max){
            max = array[i];
        }
    }

    int offset = (min < 0) ? -min : 0;
    max += offset;

    for(int exp = 1; max / exp > 0; exp *= 10){
        counting_sort_by_digit(array, size, exp, offset);
    }
}

static void bucket_insert(Node **bucket, int value){
    Node *new_node = (Node *)malloc(sizeof(Node));
    if(new_node == NULL){
        return;
    }
    new_node->data = value;
    new_node->next = *bucket;
    *bucket = new_node;
}

static void bucket_insertion_sort(Node **bucket){
    Node *sorted = NULL;
    Node *current = *bucket;

    while(current != NULL){
        Node *next = current->next;

        if(sorted == NULL || current->data <= sorted->data){
            current->next = sorted;
            sorted = current;
        }
        else{
            Node *scan = sorted;
            while(scan->next != NULL && scan->next->data < current->data){
                scan = scan->next;
            }
            current->next = scan->next;
            scan->next = current;
        }

        current = next;
    }

    *bucket = sorted;
}

void bucket_sort(int *array, int size){
    if(size <= 1){
        return;
    }

    int min = array[0];
    int max = array[0];
    for(int i = 1; i < size; i++){
        if(array[i] < min){
            min = array[i];
        }
        if(array[i] > max){
            max = array[i];
        }
    }

    Node *buckets[NUM_BUCKETS] = {0};
    int range = max - min + 1;
    if(range <= 0){
        range = 1;
    }

    for(int i = 0; i < size; i++){
        int index = (int)(((long long)(array[i] - min) * (NUM_BUCKETS - 1)) / range);
        if(index < 0){
            index = 0;
        }
        if(index >= NUM_BUCKETS){
            index = NUM_BUCKETS - 1;
        }
        bucket_insert(&buckets[index], array[i]);
    }

    for(int i = 0; i < NUM_BUCKETS; i++){
        bucket_insertion_sort(&buckets[i]);
    }

    int out = 0;
    for(int i = 0; i < NUM_BUCKETS; i++){
        Node *current = buckets[i];
        while(current != NULL){
            array[out++] = current->data;
            Node *temp = current;
            current = current->next;
            free(temp);
        }
    }
}

static int compare_ints(const void *a, const void *b){
    const int *left = (const int *)a;
    const int *right = (const int *)b;
    if(*left < *right){
        return -1;
    }
    if(*left > *right){
        return 1;
    }
    return 0;
}

void libc_qsort_wrapper(int *array, int size){
    qsort(array, (size_t)size, sizeof(int), compare_ints);
}

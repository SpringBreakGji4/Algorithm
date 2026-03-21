#ifndef SORTING_H
#define SORTING_H

#include <stddef.h>

#define MAX_INPUT_SIZE 50000
#define NUM_BUCKETS 500

void bubble_sort(int *array, int size);
void selection_sort(int *array, int size);
void insertion_sort(int *array, int size);
void shell_sort(int *array, int size);
void merge_sort(int *array, int size);
void heap_sort(int *array, int size);
void quick_sort(int *array, int size);
void radix_sort(int *array, int size);
void bucket_sort(int *array, int size);
void libc_qsort_wrapper(int *array, int size);

int load_test_case(const char *path, int *buffer, int max_size);
int check_array_sorted(const int *array, int size);
long long current_timestamp_us(void);
void print_array(const int *array, int size);

#endif

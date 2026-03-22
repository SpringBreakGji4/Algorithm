#ifndef FENWICK_H
#define FENWICK_H

typedef struct {
    int *tree;
    int *values;
    int size;
} FenwickTree;

void fenwick_init(FenwickTree *fenwick);
void fenwick_build(FenwickTree *fenwick, const int *values, int count);
void fenwick_update(FenwickTree *fenwick, int index, int delta);
void fenwick_set(FenwickTree *fenwick, int index, int value);
int fenwick_prefix_sum(FenwickTree *fenwick, int index);
int fenwick_range_sum(FenwickTree *fenwick, int left, int right);
void fenwick_print_values(FenwickTree *fenwick);
void fenwick_free(FenwickTree *fenwick);

#endif

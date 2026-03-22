#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

typedef struct {
    int *tree;
    int *values;
    int size;
} SegmentTree;

void segment_tree_init(SegmentTree *segment_tree);
void segment_tree_build(SegmentTree *segment_tree, const int *values, int count);
int segment_tree_query(SegmentTree *segment_tree, int left, int right);
void segment_tree_update(SegmentTree *segment_tree, int index, int delta);
void segment_tree_set(SegmentTree *segment_tree, int index, int value);
void segment_tree_print_values(SegmentTree *segment_tree);
void segment_tree_free(SegmentTree *segment_tree);

#endif

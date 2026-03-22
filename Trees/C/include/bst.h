#ifndef BST_H
#define BST_H

typedef struct BSTNode {
    int value;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

BSTNode *bst_insert(BSTNode *root, int value);
int bst_search(BSTNode *root, int value);
BSTNode *bst_delete(BSTNode *root, int value);
BSTNode *bst_find_min(BSTNode *root);
BSTNode *bst_find_max(BSTNode *root);
void bst_inorder(BSTNode *root);
void bst_preorder(BSTNode *root);
void bst_postorder(BSTNode *root);
int bst_height(BSTNode *root);
void bst_free(BSTNode *root);

#endif

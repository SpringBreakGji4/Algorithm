#ifndef AVL_H
#define AVL_H

typedef struct AVLNode {
    int value;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

AVLNode *avl_insert(AVLNode *node, int value);
AVLNode *avl_delete(AVLNode *node, int value);
int avl_search(AVLNode *node, int value);
void avl_inorder(AVLNode *node);
void avl_preorder(AVLNode *node);
int avl_height_value(AVLNode *node);
void avl_free(AVLNode *node);

#endif

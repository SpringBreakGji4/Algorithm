#include "bst.h"

#include <stdio.h>
#include <stdlib.h>

static BSTNode *bst_create_node(int value){
    BSTNode *node = (BSTNode *)malloc(sizeof(BSTNode));
    if(node == NULL){
        return NULL;
    }
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

BSTNode *bst_insert(BSTNode *root, int value){
    if(root == NULL){
        return bst_create_node(value);
    }
    if(value < root->value){
        root->left = bst_insert(root->left, value);
    }
    else if(value > root->value){
        root->right = bst_insert(root->right, value);
    }
    return root;
}

int bst_search(BSTNode *root, int value){
    BSTNode *current = root;
    while(current != NULL){
        if(value == current->value){
            return 1;
        }
        current = value < current->value ? current->left : current->right;
    }
    return 0;
}

BSTNode *bst_find_min(BSTNode *root){
    BSTNode *current = root;
    while(current != NULL && current->left != NULL){
        current = current->left;
    }
    return current;
}

BSTNode *bst_find_max(BSTNode *root){
    BSTNode *current = root;
    while(current != NULL && current->right != NULL){
        current = current->right;
    }
    return current;
}

BSTNode *bst_delete(BSTNode *root, int value){
    if(root == NULL){
        return NULL;
    }

    if(value < root->value){
        root->left = bst_delete(root->left, value);
    }
    else if(value > root->value){
        root->right = bst_delete(root->right, value);
    }
    else{
        if(root->left == NULL){
            BSTNode *right = root->right;
            free(root);
            return right;
        }
        if(root->right == NULL){
            BSTNode *left = root->left;
            free(root);
            return left;
        }

        BSTNode *successor = bst_find_min(root->right);
        root->value = successor->value;
        root->right = bst_delete(root->right, successor->value);
    }

    return root;
}

static void print_value(int value, int *first){
    if(!*first){
        printf(" ");
    }
    printf("%d", value);
    *first = 0;
}

static void bst_inorder_internal(BSTNode *root, int *first){
    if(root == NULL){
        return;
    }
    bst_inorder_internal(root->left, first);
    print_value(root->value, first);
    bst_inorder_internal(root->right, first);
}

void bst_inorder(BSTNode *root){
    int first = 1;
    bst_inorder_internal(root, &first);
    if(first){
        printf("(empty)");
    }
}

static void bst_preorder_internal(BSTNode *root, int *first){
    if(root == NULL){
        return;
    }
    print_value(root->value, first);
    bst_preorder_internal(root->left, first);
    bst_preorder_internal(root->right, first);
}

void bst_preorder(BSTNode *root){
    int first = 1;
    bst_preorder_internal(root, &first);
    if(first){
        printf("(empty)");
    }
}

static void bst_postorder_internal(BSTNode *root, int *first){
    if(root == NULL){
        return;
    }
    bst_postorder_internal(root->left, first);
    bst_postorder_internal(root->right, first);
    print_value(root->value, first);
}

void bst_postorder(BSTNode *root){
    int first = 1;
    bst_postorder_internal(root, &first);
    if(first){
        printf("(empty)");
    }
}

int bst_height(BSTNode *root){
    if(root == NULL){
        return -1;
    }
    int left_height = bst_height(root->left);
    int right_height = bst_height(root->right);
    return 1 + (left_height > right_height ? left_height : right_height);
}

void bst_free(BSTNode *root){
    if(root == NULL){
        return;
    }
    bst_free(root->left);
    bst_free(root->right);
    free(root);
}

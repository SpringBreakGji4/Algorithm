#include "avl.h"

#include <stdio.h>
#include <stdlib.h>

static int max(int a, int b){
    return a > b ? a : b;
}

static int node_height(AVLNode *node){
    return node == NULL ? 0 : node->height;
}

static AVLNode *create_node(int value){
    AVLNode *node = (AVLNode *)malloc(sizeof(AVLNode));
    if(node == NULL){
        return NULL;
    }
    node->value = value;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static void update_height(AVLNode *node){
    node->height = 1 + max(node_height(node->left), node_height(node->right));
}

static int balance_factor(AVLNode *node){
    return node == NULL ? 0 : node_height(node->left) - node_height(node->right);
}

static AVLNode *rotate_right(AVLNode *y){
    AVLNode *x = y->left;
    AVLNode *t2 = x->right;

    x->right = y;
    y->left = t2;

    update_height(y);
    update_height(x);
    return x;
}

static AVLNode *rotate_left(AVLNode *x){
    AVLNode *y = x->right;
    AVLNode *t2 = y->left;

    y->left = x;
    x->right = t2;

    update_height(x);
    update_height(y);
    return y;
}

AVLNode *avl_insert(AVLNode *node, int value){
    if(node == NULL){
        return create_node(value);
    }

    if(value < node->value){
        node->left = avl_insert(node->left, value);
    }
    else if(value > node->value){
        node->right = avl_insert(node->right, value);
    }
    else{
        return node;
    }

    update_height(node);
    int balance = balance_factor(node);

    if(balance > 1 && value < node->left->value){
        return rotate_right(node);
    }
    if(balance < -1 && value > node->right->value){
        return rotate_left(node);
    }
    if(balance > 1 && value > node->left->value){
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    if(balance < -1 && value < node->right->value){
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

static AVLNode *min_node(AVLNode *node){
    AVLNode *current = node;
    while(current != NULL && current->left != NULL){
        current = current->left;
    }
    return current;
}

AVLNode *avl_delete(AVLNode *node, int value){
    if(node == NULL){
        return NULL;
    }

    if(value < node->value){
        node->left = avl_delete(node->left, value);
    }
    else if(value > node->value){
        node->right = avl_delete(node->right, value);
    }
    else{
        if(node->left == NULL || node->right == NULL){
            AVLNode *temp = node->left != NULL ? node->left : node->right;
            if(temp == NULL){
                free(node);
                return NULL;
            }
            *node = *temp;
            free(temp);
        }
        else{
            AVLNode *temp = min_node(node->right);
            node->value = temp->value;
            node->right = avl_delete(node->right, temp->value);
        }
    }

    update_height(node);
    int balance = balance_factor(node);

    if(balance > 1 && balance_factor(node->left) >= 0){
        return rotate_right(node);
    }
    if(balance > 1 && balance_factor(node->left) < 0){
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    if(balance < -1 && balance_factor(node->right) <= 0){
        return rotate_left(node);
    }
    if(balance < -1 && balance_factor(node->right) > 0){
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

int avl_search(AVLNode *node, int value){
    AVLNode *current = node;
    while(current != NULL){
        if(value == current->value){
            return 1;
        }
        current = value < current->value ? current->left : current->right;
    }
    return 0;
}

static void print_value(int value, int *first){
    if(!*first){
        printf(" ");
    }
    printf("%d", value);
    *first = 0;
}

static void avl_inorder_internal(AVLNode *node, int *first){
    if(node == NULL){
        return;
    }
    avl_inorder_internal(node->left, first);
    print_value(node->value, first);
    avl_inorder_internal(node->right, first);
}

void avl_inorder(AVLNode *node){
    int first = 1;
    avl_inorder_internal(node, &first);
    if(first){
        printf("(empty)");
    }
}

static void avl_preorder_internal(AVLNode *node, int *first){
    if(node == NULL){
        return;
    }
    print_value(node->value, first);
    avl_preorder_internal(node->left, first);
    avl_preorder_internal(node->right, first);
}

void avl_preorder(AVLNode *node){
    int first = 1;
    avl_preorder_internal(node, &first);
    if(first){
        printf("(empty)");
    }
}

int avl_height_value(AVLNode *node){
    return node == NULL ? -1 : node->height - 1;
}

void avl_free(AVLNode *node){
    if(node == NULL){
        return;
    }
    avl_free(node->left);
    avl_free(node->right);
    free(node);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
    char* data;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct BST {
    Node* root;
} BST;

Node* create_node(char* value){
    Node* new_node = (Node*)malloc(sizeof(Node));

    new_node->data = strdup(value);
    new_node->left = new_node->right = NULL;

    return new_node;
}

BST* create_bst(){
    BST* tree = (BST*)malloc(sizeof(BST));
    tree->root = NULL;
    return tree;
}

Node* insert_node(Node* node, char* value){
    if(node == NULL) {
        return create_node(value);
    }

    if (strcmp(value, node->data) < 0)
        node->left = insert_node(node->left, value);
    else
        node->right = insert_node(node->right, value);

    return node;
}

void bst_insert(BST* tree, char* value){
    tree->root = insert_node(tree->root, value);
}

int search_node(Node* node, char* value, int* index){
    if(node == NULL){
        return -1;
    }

    int left_result = search_node(node->left, value, index);
    if(left_result != -1)
        return left_result;

    if(strcmp(node->data, value) == 0){
        return *index;
    }
    (*index)++;

    return search_node(node->right, value, index);
}

int bst_search(BST* tree, char* value){
    int index = 0;
    return search_node(tree->root, value, &index);
}

void bst_output(Node* root){
    if (root == NULL)
        return;

    bst_output(root->left);
    printf("%s ", root->data);
    bst_output(root->right);
}

void free_node(Node* node){
    if(node == NULL)
        return;

    free_node(node->left);
    free_node(node->right);
    free(node->data);
    free(node);
}

void free_bst(BST* tree){
    if(tree){
        free_node(tree->root);
        free(tree);
    }
}

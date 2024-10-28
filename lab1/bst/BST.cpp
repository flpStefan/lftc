#include "BST.h"

/*
 * Public methods
 */
BinarySearchTree::BinarySearchTree() {
    this->root = nullptr;
}

void BinarySearchTree::insert(string value) {
    root = insert(root, value);
}

int BinarySearchTree::get_index(string value) {
    int index = 0;
    return search(root, value, index);
}

void BinarySearchTree::inorder_output(Node *root, ofstream& out) {
    if(root == nullptr)
        return;

    inorder_output(root->left, out);
    out<< root->data << "\n";
    inorder_output(root->right, out);
}


/*
 * Private methods
 */
Node *BinarySearchTree::insert(Node *node, string value) {
    if (node == nullptr) {
        return new Node(value);
    }

    if (value < node->data)
        node->left = insert(node->left, value);
    else
        node->right = insert(node->right, value);

    return node;
}

int BinarySearchTree::search(Node *node, string value, int &index) {
    if (node == nullptr)
        return -1;

    int left = search(node->left, value, index);
    if (left != -1) return left;

    if (node->data == value)
        return index;

    index++;
    return search(node->right, value, index);
}

#ifndef BST_H
#define BST_H
#include <string>
#include <fstream>
using std::string;
using std::ofstream;

class Node {
public:
    string data;
    Node* left;
    Node* right;

    explicit Node(string data) {
        this->data = data;
        left = nullptr;
        right = nullptr;
    }
};


class BinarySearchTree {
public:
    Node* root;

    BinarySearchTree();
    void insert(string value);
    int get_index(string value);
    void inorder_output(Node* root, ofstream& out);
private:
    Node* insert(Node* node, string value);
    int search(Node* node, string value, int& index);
};

#endif
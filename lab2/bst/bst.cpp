#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <numeric>
#include <random>

struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int value) : data(value), left(nullptr), right(nullptr) {}
};

class BST {
public:
    Node* root;

    BST() : root(nullptr) {}

    void insert(int value) {
        root = insertRecursive(root, value);
    }

    Node* insertRecursive(Node* node, int value) {
        if (node == nullptr) {
            return new Node(value);
        }
        if (value < node->data) {
            node->left = insertRecursive(node->left, value);
        } else if (value > node->data) {
            node->right = insertRecursive(node->right, value);
        }
        return node;
    }

    void inOrderTraversal(Node* node) {
        if (node != nullptr) {
            inOrderTraversal(node->left);
            std::cout << node->data << " ";
            inOrderTraversal(node->right);
        }
    }

    ~BST() {
        destroyTree(root);
    }

private:
    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
};

void testBST() {
    std::vector<int> sizes = {10, 100, 1000, 10000};
    for (int size : sizes) {
        std::cout << "Testing size: " << size << std::endl;
        std::vector<int> array(size);
        std::iota(array.begin(), array.end(), 1);  

        std::vector<std::vector<int>> testCases = {
            array,
            std::vector<int>(array.rbegin(), array.rend()), 
            array
        };

        std::random_device rd;
        std::mt19937 g(rd());

        std::shuffle(testCases[2].begin(), testCases[2].end(), g);  

        for (auto& testCase : testCases) {
            BST tree;
            auto start = std::chrono::high_resolution_clock::now();
            for (int value : testCase) {
                tree.insert(value);
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            std::cout << "Elapsed time: " << elapsed.count() << " seconds." << std::endl;
            tree.inOrderTraversal(tree.root);
            std::cout << std::endl << "-------------" << std::endl;
        }
    }
}

int main() {
    testBST();
    return 0;
}

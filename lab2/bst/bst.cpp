#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <numeric>
#include <random>
#include <span> 
#include <functional>

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

	bool search(int value) {
        Node* node = root;
        while (node != nullptr) {
            if (value == node->data) {
                return true;
            } else if (value < node->data) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
        return false;
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
    static void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
};

void searchAll(BST& tree, std::span<const int> data) {
    for (int value : data) {
        tree.search(value);
    }
}

void insertAll(BST& tree, std::span<const int> data) {
    for (int value : data) {
        tree.insert(value);
    }
}

void testBST(std::span<const int> sizes, int numTests,
             std::function<void(BST&, std::span<const int>)> insertFunction,
             std::function<void(BST&, std::span<const int>)> searchFunction) {
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int size : sizes) {
        std::vector<int> array(size);
        std::iota(array.begin(), array.end(), 1);  
        std::shuffle(array.begin(), array.end(), gen);

        std::vector<std::vector<int>> testCases = {
            array,
            std::vector<int>(array.rbegin(), array.rend()), 
            array
        };

        std::shuffle(testCases[2].begin(), testCases[2].end(), gen);  

        for (auto& testCase : testCases) {
            std::span<const int> testCaseSpan(testCase);
            std::vector<double> times;
            for (int test = 0; test < numTests; ++test) {
                BST tree;
                auto start = std::chrono::high_resolution_clock::now();
                insertFunction(tree, testCaseSpan);
                searchFunction(tree, testCaseSpan);
                auto end = std::chrono::high_resolution_clock::now();
                times.push_back(std::chrono::duration<double, std::milli>(end - start).count());
            }
            double avgTime = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
            std::cout << "Average time for size " << size << ": " << avgTime << " ms" << std::endl;
        }
    }
}

int main() {
    std::vector<int> sizes = {10, 100, 1000, 10000};
    int numTests = 5;  // Number of tests to average
    testBST(sizes, numTests, insertAll, searchAll);
    return 0;
}


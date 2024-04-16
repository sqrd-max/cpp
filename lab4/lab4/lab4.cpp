#include <iostream>
#include <vector>
#include <queue>
#include <set>

struct GraphNode {
    int value;
    std::vector<GraphNode*> neighbors;

    explicit GraphNode(int val) : value(val) {} // для явного преобразования типов
};

void addEdge(GraphNode* from, GraphNode* to) {
    from->neighbors.push_back(to);
}

int sumOfNeighbors(GraphNode* node) {
    int sum = 0;
    for (auto neighbor : node->neighbors) {
        sum += neighbor->value;
    }
    return sum;
}

void DFS(GraphNode* node, std::set<GraphNode*>& visited) {
    auto result = visited.emplace(node);
    if (result.second) {
        std::cout << "Visited (DFS): " << node->value << std::endl;
        for (auto neighbor : node->neighbors) {
                DFS(neighbor, visited);
        }
    }
}

void BFS(GraphNode* startNode) {
    std::queue<GraphNode*> queue;
    std::set<GraphNode*> visited;
    queue.push(startNode);
    visited.emplace(startNode);

    while (!queue.empty()) {
        GraphNode* node = queue.front();
        queue.pop();
        std::cout << "Visited (BFS): " << node->value << std::endl;

        for (auto neighbor : node->neighbors) {
            if (visited.emplace(neighbor).second) {
                queue.push(neighbor);
            }
        }
    }
}

int main() {
    GraphNode* node1 = new GraphNode(1);
    GraphNode* node2 = new GraphNode(2);
    GraphNode* node3 = new GraphNode(3);
    GraphNode* node4 = new GraphNode(4);
    
    addEdge(node1, node2);
    addEdge(node1, node3);
    addEdge(node2, node3);
    addEdge(node3, node4);
    addEdge(node4, node1);
    
    std::cout << "Sum of neighbors for node 1: " << sumOfNeighbors(node1) << std::endl;

    std::set<GraphNode*> visitedDFS;
    DFS(node1, visitedDFS);

    BFS(node1);

    // Освобождение памяти
    delete node1;
    delete node2;
    delete node3;
    delete node4;

    return 0;
}

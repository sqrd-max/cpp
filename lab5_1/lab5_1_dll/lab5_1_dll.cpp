#include <iostream>
#include <cassert>

struct Node {
    int data;
    Node* prev;
    Node* next;

    Node(int value) : data(value), prev(nullptr), next(nullptr) {}
};

struct LinkedList {
    Node* first;     // Указатель на первый узел
    Node* last;      // Указатель на последний узел

    LinkedList() : first(nullptr), last(nullptr) {}

    // Вставка нового узла после указанного узла
    Node* insertAfter(Node* node, int value) {
        Node* newNode = new Node(value);
        if (!node) {
            // Вставка в начало списка
            newNode->next = first;
            if (first) {
                first->prev = newNode;
            }
            first = newNode;
            if (!last) {
                // Список был пуст
                last = newNode;
            }
        } else {
            newNode->next = node->next;
            newNode->prev = node;
            if (node->next) {
                node->next->prev = newNode;
            } else {
                // Обновление последнего узла, если новый узел вставлен после последнего узла
                last = newNode;
            }
            node->next = newNode;
        }

        return newNode;
    }

    // Вставка нового узла перед указанным узлом
    Node* insertBefore(Node* node, int value) {
        Node* newNode = new Node(value);
        if (!node) {
            // Вставка в конец списка
            newNode->prev = last;
            if (last) {
                last->next = newNode;
            }
            last = newNode;
            if (!first) {
                // Список был пуст
                first = newNode;
            }
        } else {
            newNode->prev = node->prev;
            newNode->next = node;
            if (node->prev) {
                node->prev->next = newNode;
            } else {
                // Обновление первого узла, если новый узел вставлен перед первым узлом
                first = newNode;
            }
            node->prev = newNode;
        }

        return newNode;
    }

    // Поиск узла с заданным значением
    Node* find(int value) {
        Node* current = first;
        while (current) {
            if (current->data == value) {
                return current;
            }
            current = current->next;
        }

        return nullptr;
    }

    // Удаление указанного узла
    void remove(Node* node) {
        if (!node) {
            return;
        }

        if (node->prev) {
            node->prev->next = node->next;
        } else {
            // Обновление первого узла, если удаляется первый узел
            first = node->next;
        }

        if (node->next) {
            node->next->prev = node->prev;
        } else {
            // Обновление последнего узла, если удаляется последний узел
            last = node->prev;
        }

        delete node;
    }

    // Проверка отсутствия циклов в списке
    void assertNoCycles() {
        Node* current = first;
        while (current) {
            if (current->next && current->next->prev != current) {
                std::cerr << "Error: Inconsistency in pointers in a doubly linked list\n";
                assert(false);
            }
            current = current->next;
        }
    }

    // Деструктор для освобождения памяти
    ~LinkedList() {
        Node* current = first;
        while (current) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        first = last = nullptr;
    }
};

int main() {
    LinkedList list;

    // Вставка узлов
    Node* node1 = list.insertAfter(nullptr, 10);
    Node* node2 = list.insertAfter(node1, 20);
    Node* node3 = list.insertBefore(node2, 15);
    Node* node4 = list.insertAfter(node3, 30);

    // Поиск узлов
    Node* foundNode = list.find(20);
    assert(foundNode == node2);

    // Удаление узла
    list.remove(node3);

    // Проверка отсутствия циклов
    list.assertNoCycles();

    // Очистка (деструктор будет вызван)

    std::cout << "All tests passed!" << std::endl;

    return 0;
}

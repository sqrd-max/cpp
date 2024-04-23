#include <iostream>
#include <array>
#include <optional>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <random>
#include <string>
#include <span>
#include "Person.h"

struct Node {
    Person data;
    Node* next = nullptr;
};

struct LinkedList {
    Node* head = nullptr;
    size_t size = 0; // Хранение размера списка для удобства
};

Node* insertAfter(LinkedList* list, Node* node, const Person& value) {
    Node* newNode = new Node{value, nullptr};
    if (!node) { // Вставка в начало списка
        newNode->next = list->head;
        list->head = newNode;
    } else { // Вставка после указанного узла
        newNode->next = node->next;
        node->next = newNode;
    }
    list->size++;
    return newNode;
}

Node* find(LinkedList* list, int age) {
    Node* current = list->head;
    while (current != nullptr) {
        if (current->data.age == age) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

void removeAfter(LinkedList* list, Node* node) {
    if (!node) { // Удаление первого узла
        if (list->head) {
            Node* toDelete = list->head;
            list->head = list->head->next;
            delete toDelete;
            list->size--;
        }
    } else if (node->next) {
        Node* toDelete = node->next;
        node->next = node->next->next;
        delete toDelete;
        list->size--;
    }
}

void assertNoCycles(LinkedList* list) {
    size_t count = 0;
    Node* current = list->head;
    while (current != nullptr && count <= list->size) {
        current = current->next;
        count++;
    }
    if (count > list->size) {
        std::cerr << "Cycle detected in the list!" << std::endl;
        exit(1);
    }
}
void testLinkedList() {
    LinkedList list;

    // Добавляем элементы в список
    Person person1 = { "Alice", 30, Gender::Female };
    Person person2 = { "Bob", 25, Gender::Male };
    insertAfter(&list, nullptr, person1); // Вставка в начало
    insertAfter(&list, list.head, person2); // Вставка после первого элемента

    // Поиск
    Node* found = find(&list, 25);
    if (found) {
        std::cout << "Found: " << found->data.name.data() << std::endl;
    } else {
        std::cout << "Not found" << std::endl;
    }

    // Удаление
    removeAfter(&list, nullptr); // Удаление первого узла
    assertNoCycles(&list); // Проверка на циклы

    
}

int main() {
    testLinkedList();
    return 0;
}

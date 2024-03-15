#include <iostream>
#include <array>
#include <optional>

enum class Gender { Male, Female };

struct Person {
    std::array<char, 50> name;
    int age;
    Gender gender;
};

struct DNode {
    Person data;
    DNode* prev = nullptr;
    DNode* next = nullptr;
};

struct DoublyLinkedList {
    DNode* head = nullptr;
    DNode* tail = nullptr;
    size_t size = 0;
};

// Вставка после указанного узла
DNode* insertAfter(DoublyLinkedList* list, DNode* node, const Person& value) {
    DNode* newNode = new DNode{value, nullptr, nullptr};
    if (!node) {
        if (list->head == nullptr) {
            list->head = list->tail = newNode;
        } else {
            newNode->next = list->head;
            list->head->prev = newNode;
            list->head = newNode;
        }
    } else {
        newNode->next = node->next;
        newNode->prev = node;
        node->next = newNode;
        if (newNode->next != nullptr) {
            newNode->next->prev = newNode;
        } else {
            list->tail = newNode;
        }
    }
    list->size++;
    return newNode;
}

// Вставка перед указанным узлом
DNode* insertBefore(DoublyLinkedList* list, DNode* node, const Person& value) {
    if (!node || node == list->head) {
        return insertAfter(list, nullptr, value);
    } else {
        DNode* newNode = new DNode{value, nullptr, nullptr};
        newNode->prev = node->prev;
        newNode->next = node;
        node->prev->next = newNode;
        node->prev = newNode;
        list->size++;
        return newNode;
    }
}

// Поиск узла по возрасту
DNode* find(DoublyLinkedList* list, int age) {
    DNode* current = list->head;
    while (current != nullptr) {
        if (current->data.age == age) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// Удаление узла
void remove(DoublyLinkedList* list, DNode* node) {
    if (!node) return;
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }
    delete node;
    list->size--;
}

// Функция для тестирования списка
void testDoublyLinkedList() {
    DoublyLinkedList list;

    // Добавление элементов в список
    Person alice = {"Alice", 30, Gender::Female};
    Person bob = {"Bob", 25, Gender::Male};
    insertAfter(&list, nullptr, alice);
    insertAfter(&list, list.head, bob);

    // Поиск
    DNode* found = find(&list, 25);
    if (found) {
        std::cout << "Found person: " << found->data.name.data() << std::endl;
    } else {
        std::cout << "Person not found." << std::endl;
    }

    // Удаление
    remove(&list, found);
    std::cout << "After removal, size of list: " << list.size << std::endl;
}

int main() {
    testDoublyLinkedList();
    return 0;
}

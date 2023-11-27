#include <iostream>
#include <cassert>

struct Node {
    int data;
    Node* next;

    Node(int value) : data(value), next(nullptr) {}
};

struct LinkedList {
    Node* first;     // Указатель на первый узел
    Node* last;      // Указатель на последний узел
    size_t size;     // Текущее количество узлов в списке

    LinkedList() : first(nullptr), last(nullptr), size(0) {}

    // Вставка нового узла после указанного узла
    Node* insertAfter(Node* node, int value) {
        Node* newNode = new Node(value);
        if (!node) {
            // Вставка в начало списка
            newNode->next = first;
            first = newNode;
            if (!last) {
                // Список был пуст
                last = newNode;
            }
        } else {
            newNode->next = node->next;
            node->next = newNode;
            if (node == last) {
                // Обновление последнего узла, если новый узел вставлен после последнего узла
                last = newNode;
            }
        }
        ++size;
        return newNode;
    }

    // Структура для результата поиска узла по значению
    struct FindNodeResult {
        Node* node;      // Узел с заданным значением
        Node* previous;  // Узел перед ним
    };

    // Поиск узла с заданным значением
    FindNodeResult find(int value) {
        FindNodeResult result = { nullptr, nullptr };
        Node* current = first;
        Node* previous = nullptr;

        while (current) {
            if (current->data == value) {
                result.node = current;
                result.previous = previous;
                break;
            }
            previous = current;
            current = current->next;
        }

        return result;
    }

    // Удаление узла после указанного узла
    void removeAfter(Node* node) {
        if (!node) {
            // Удаление первого узла
            if (first) {
                Node* temp = first;
                first = first->next;
                delete temp;
                --size;
                if (!first) {
                    // Список стал пустым
                    last = nullptr;
                }
            }
        } else if (node->next) {
            // Удаление узла после указанного узла
            Node* temp = node->next;
            node->next = temp->next;
            delete temp;
            --size;
            if (!node->next) {
                // Обновление последнего узла, если удаленный узел был последним
                last = node;
            }
        }
    }

    // Проверка отсутствия циклов в списке
    void assertNoCycles() {
        size_t count = 0;
        Node* slow = first;
        Node* fast = first;

        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            ++count;

            assert(count <= size);  // Убедитесь, что count не превышает фактического размера списка

            if (slow == fast) {
                // Обнаружен цикл
                std::cerr << "Error: Cycle detected in the linked list.\n";
                assert(false);
            }
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
        size = 0;
    }
};

int main() {
    LinkedList list;

    // Вставка узлов
    Node* node1 = list.insertAfter(nullptr, 10);
    Node* node2 = list.insertAfter(node1, 20);
    Node* node3 = list.insertAfter(node2, 30);

    // Поиск узлов
    LinkedList::FindNodeResult result = list.find(20);
    assert(result.node == node2);
    assert(result.previous == node1);

    // Удаление узлов
    list.removeAfter(node1);

    // Проверка отсутствия циклов
    list.assertNoCycles();

    // Очистка (деструктор будет вызван)

    std::cout << "All tests passed!" << std::endl;

    return 0;
}

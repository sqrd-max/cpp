#include <iostream>
#include <cassert>

struct Node {
    int data;
    Node* next;

    Node(int value) : data(value), next(nullptr) {}
};

struct Queue {
    Node* front;  // Указатель на первый узел (начало очереди)
    Node* rear;   // Указатель на последний узел (конец очереди)

    Queue() : front(nullptr), rear(nullptr) {}

    // Добавление элемента в конец очереди
    void enqueue(int value) {
        Node* newNode = new Node(value);
        if (isEmpty()) {
            // Если очередь пуста, новый узел становится как начало, так и концом очереди
            front = rear = newNode;
        } else {
            // В противном случае добавляем узел в конец и обновляем указатель rear
            rear->next = newNode;
            rear = newNode;
        }
    }

    // Удаление элемента из начала очереди и возвращение его значения
    int dequeue() {
        if (isEmpty()) {
            std::cerr << "Error: Queue is empty. Unable to execute dequeue.\n";
            assert(false);  // Можно использовать исключение вместо assert
        }

        // Получаем значение из первого узла и удаляем его
        int value = front->data;
        Node* temp = front;
        front = front->next;
        delete temp;

        // Если после удаления очередь стала пустой, обновляем указатель rear
        if (isEmpty()) {
            rear = nullptr;
        }

        return value;
    }

    // Проверка, является ли очередь пустой
    bool isEmpty() const {
        return front == nullptr;
    }

    // Деструктор для освобождения памяти
    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }
};

int main() {
    Queue queue;

    // Добавление элементов в очередь
    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);

    // Удаление и вывод элементов из очереди
    while (!queue.isEmpty()) {
        int value = queue.dequeue();
        std::cout << "Dequeued: " << value << std::endl;
    }

    return 0;
}

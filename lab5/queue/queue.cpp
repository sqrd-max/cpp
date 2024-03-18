#include <iostream>
#include <string>
#include <optional>

enum class Gender {Male, Female};

struct Person {
    std::string name;
    int age;
    Gender gender;
};

struct Node {
    Person data;
    Node* next = nullptr;
};

class Queue {
private:
    Node* head;
    Node* tail;

public:
    Queue() : head(nullptr), tail(nullptr) {}

    ~Queue() {
        while (head != nullptr) {
            dequeue();
        }
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    void enqueue(const Person& value) {
        Node* newNode = new Node{value, nullptr};
        if (tail != nullptr) {
            tail->next = newNode;
        }
        tail = newNode;
        if (head == nullptr) {
            head = newNode;
        }
    }

    Person* front() {
        if (!isEmpty()) {
            return &head->data;
        }
        return nullptr;
    }

    void dequeue() {
        if (head != nullptr) {
            Node* temp = head;
            head = head->next;
            if (head == nullptr) {
                tail = nullptr;
            }
            delete temp;
        }
    }
};

int main() {
    Queue queue;

    queue.enqueue({"Alice", 30, Gender::Female});
    queue.enqueue({"Bob", 25, Gender::Male});

    while (!queue.isEmpty()) {
        Person* person = queue.front();
        if (person) {
            std::cout << "Front of queue: " << person->name << ", Age: " << person->age << std::endl;
        }
        queue.dequeue();
    }

    return 0;
}

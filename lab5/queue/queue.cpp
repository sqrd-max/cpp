#include <cassert>
#include <iostream>
#include <string>
#include <optional>
#include <vector>

enum class Gender {Male, Female};

struct Person {
    std::string name;
    int age;
    Gender gender;
};

class Queue {
private:
    std::vector<std::optional<Person>> buffer;
    size_t head = 0;
    size_t tail = 0;
    size_t count = 0;
    size_t capacity;

    size_t nextIndex(size_t index) const {
        return (index + 1) % capacity;
    }

public:
    explicit Queue(size_t cap = 10) : buffer(cap), capacity(cap) {}

    bool isEmpty() const {
        return count == 0;
    }

    bool isFull() const {
        return count == capacity;
    }

    void enqueue(const Person& value) {
        assert(!isFull());
        buffer[tail] = value;
        tail = nextIndex(tail);
        ++count;
    }

    void enqueue(Person&& value) {
        assert(!isFull());
        buffer[tail] = std::move(value);
        tail = nextIndex(tail);
        ++count;
    }

    Person front() {
        assert(!isEmpty()); 
        return *buffer[head];
    }

    void dequeue() {
        assert(!isEmpty());
        buffer[head].reset();
        head = nextIndex(head);
        --count;
    }
};

int main() {
    Queue queue(5);

    queue.enqueue({"Alice", 30, Gender::Female});
    queue.enqueue(Person{"Bob", 25, Gender::Male}); // Использование перемещения

    while (!queue.isEmpty()) {
        auto person = queue.front();
        std::cout << "Front of queue: " << person.name << ", Age: " << person.age << std::endl;
        queue.dequeue();
    }

    return 0;
}

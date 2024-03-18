#include <iostream>
#include <vector>
#include <string>

enum class Gender {Male, Female};

struct Person {
    std::string name;
    int age;
    Gender gender;
};

class Stack {
private:
    std::vector<Person> elements;

public:
    bool isEmpty() const {
        return elements.empty();
    }

    void push(const Person& value) {
        elements.push_back(value);
    }

    // Возвращает указатель на последний элемент или nullptr, если стек пустой
    Person* getLastElement() {
        if (!isEmpty()) {
            return &elements.back();
        }
        return nullptr;
    }

    void pop() {
        if (!isEmpty()) {
            elements.pop_back();
        }
    }
};

int main() {
    Stack stack;
    
    stack.push({"Alice", 30, Gender::Female});
    stack.push({"Bob", 25, Gender::Male});

    if (!stack.isEmpty()) {
        Person* topPerson = stack.getLastElement();
        if (topPerson) {
            std::cout << "Top person on stack: " << topPerson->name << ", Age: " << topPerson->age << std::endl;
        }
        stack.pop();
    }

    if (!stack.isEmpty()) {
        Person* topPerson = stack.getLastElement();
        if (topPerson) {
            std::cout << "Top person on stack: " << topPerson->name << ", Age: " << topPerson->age << std::endl;
        }
    } else {
        std::cout << "Stack is now empty." << std::endl;
    }

    return 0;
}

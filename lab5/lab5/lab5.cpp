#include <iostream>
#include <vector>
#include <string>
#include <cassert>

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

    void push(Person&& value) {
        elements.push_back(std::move(value)); // Используем std::move для перемещения
    }

    // Возвращает указатель на последний элемент или nullptr, если стек пустой
    Person* getLastElement() {
        assert(!isEmpty());
        return &elements.back();
    }

    void pop() {
        assert(!isEmpty());
        elements.pop_back();
    }
};

int main() {
    Stack stack{};
    
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

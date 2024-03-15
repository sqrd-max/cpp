#include <iostream>
#include <cassert>

template <typename T>
class DynamicArray {
private:
    T* data;
    size_t length;
    size_t capacity;

public:
    // Default constructor
    DynamicArray() : data(nullptr), length(0), capacity(0) {}

    // Constructor with capacity parameter
    explicit DynamicArray(size_t capacity) : data(new T[capacity]), length(0), capacity(capacity) {}

    // Destructor
    ~DynamicArray() {
        delete[] data;
    }

    // Copy constructor
    DynamicArray(const DynamicArray& other) : data(new T[other.capacity]), length(other.length), capacity(other.capacity) {
        for (size_t i = 0; i < other.length; ++i) {
            data[i] = other.data[i];
        }
    }

    // Copy assignment operator
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;
            data = new T[other.capacity];
            length = other.length;
            capacity = other.capacity;
            for (size_t i = 0; i < other.length; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // Move constructor
    DynamicArray(DynamicArray&& other) noexcept : data(other.data), length(other.length), capacity(other.capacity) {
        other.data = nullptr;
        other.length = 0;
        other.capacity = 0;
    }

    // Index operator
    T& operator[](size_t index) {
        assert(index < length);
        return data[index];
    }

    // Method to add an element
    void add(const T& element) {
        if (length == capacity) {
            resizeArray(capacity * 2);
        }
        data[length++] = element;
    }

    // Method to get the size (read-only property)
    size_t size() const {
        return length;
    }

    // Method to get the capacity (read-only property)
    size_t capacity() const {
        return capacity;
    }

    // Iterator methods
    T* begin() const {
        return data;
    }

    T* end() const {
        return data + length;
    }

private:
    // Private method to resize the array
    void resizeArray(size_t newCapacity) {
        T* newData = new T[newCapacity];
        for (size_t i = 0; i < length; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }
};

int main() {
    // Test constructor with capacity parameter
    DynamicArray<int> arrWithCapacity(10);
    assert(arrWithCapacity.capacity() == 10);

    // Test default constructor
    DynamicArray<int> arrDefault;
    assert(arrDefault.size() == 0);

    // Test add method
    arrDefault.add(5);
    assert(arrDefault.size() == 1);

    // Test copy constructor
    DynamicArray<int> arrCopy = arrDefault;
    assert(arrCopy.size() == 1);
    assert(arrCopy[0] == 5);

    // Test copy assignment operator
    DynamicArray<int> arrCopyAssign;
    arrCopyAssign = arrDefault;
    assert(arrCopyAssign.size() == 1);
    assert(arrCopyAssign[0] == 5);

    // Test move constructor
    DynamicArray<int> arrMove = std::move(arrDefault);
    assert(arrMove.size() == 1);
    assert(arrMove[0] == 5);
    assert(arrDefault.size() == 0);  // arrDefault should be moved-from

    // Test index operator
    assert(arrCopy[0] == 5);

    // Test iterator
    int sum = 0;
    for (int value : arrCopy) {
        sum += value;
    }
    assert(sum == 5);

    std::cout << "All tests passed!" << std::endl;

    return 0;
}

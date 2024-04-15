#include <iostream>
#include <memory>
#include <span>

struct Iliffe {
    int** data;
    size_t* dimensions;
    size_t num_dimensions;
};

Iliffe createIliffe(std::span<size_t> sizes) {
    Iliffe array;
    array.num_dimensions = sizes.size();
    array.dimensions = new size_t[array.num_dimensions];
    std::copy(sizes.begin(), sizes.end(), array.dimensions);

    array.data = new int*[sizes[0]];  // создаем массив указателей для строк
    for (size_t i = 0; i < sizes[0]; ++i) {
        array.data[i] = new int[sizes[1]];  // создаем строки
    }
    return array;
}

int* getElement(Iliffe* array, std::span<size_t> indices) {
    if (indices.size() != array->num_dimensions) {
        throw std::runtime_error("Index dimension mismatch");
    }
    return &(array->data[indices[0]][indices[1]]);
}

int main() {
    size_t dimensions[] = {3, 4};
    Iliffe myArray = createIliffe(std::span<size_t>(dimensions, 2));  // Создаем span из массива

    size_t indices[] = {2, 3};  // Используем массив для индексов
    int* elem = getElement(&myArray, std::span<size_t>(indices, 2));  // Создаем span из массива индексов
    *elem = 10;
    std::cout << "Element at (2, 3): " << *elem << std::endl;

    // Очистка памяти
    for (size_t i = 0; i < myArray.dimensions[0]; ++i) {
        delete[] myArray.data[i];
    }
    delete[] myArray.data;
    delete[] myArray.dimensions;
}

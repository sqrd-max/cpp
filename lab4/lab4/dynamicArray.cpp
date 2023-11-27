// dynamicArray.cpp

#include "dynamicArray.h"
#include <cassert>

namespace {

    void resizeArray(DynamicArray* arr, size_t newCapacity) {
        int* newData = new int[newCapacity];
        for (size_t i = 0; i < arr->length; ++i) {
            newData[i] = arr->data[i];
        }
        delete[] arr->data;
        arr->data = newData;
        arr->capacity = newCapacity;
    }

} // anonymous namespace

DynamicArray createDynamicArrayWithCapacity(size_t capacity) {
    DynamicArray arr;
    arr.data = new int[capacity];
    arr.length = 0;
    arr.capacity = capacity;
    return arr;
}

DynamicArray createDynamicArray() {
    return createDynamicArrayWithCapacity(4);
}

void addElementToArray(DynamicArray* arr, int element) {
    if (arr->length == arr->capacity) {
        // Удваиваем длину массива, если он полный
        resizeArray(arr, arr->capacity * 2);
    }
    arr->data[arr->length++] = element;
}

int getElementAtIndex(const DynamicArray* arr, size_t index) {
    assert(index < arr->length);
    return arr->data[index];
}

void getCurrentSpan(const DynamicArray* arr, int*& start, size_t& size) {
    start = arr->data;
    size = arr->length;
}

void destroyDynamicArray(DynamicArray* arr) {
    delete[] arr->data;
    arr->data = nullptr;
    arr->length = 0;
    arr->capacity = 0;
}

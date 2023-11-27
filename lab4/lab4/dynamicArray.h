#pragma once

struct DynamicArray {
    int* data;
    size_t length;
    size_t capacity;
};

DynamicArray createDynamicArrayWithCapacity(size_t capacity);
DynamicArray createDynamicArray();
void addElementToArray(DynamicArray* arr, int element);
int getElementAtIndex(const DynamicArray* arr, size_t index);
void getCurrentSpan(const DynamicArray* arr, int*& start, size_t& size);
void destroyDynamicArray(DynamicArray* arr);

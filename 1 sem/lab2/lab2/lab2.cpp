#include <iostream>

int main() {
    int i = 69;
    int number = 72;
    
    i = 69 + 72;
    
    int group[3] = {0, 0, 0};
    
    int* iPointer = &i;
    
    number = *iPointer;
    
    *iPointer = 15;
    
    iPointer = &group[0];
    iPointer += 2; // Перемещаем указатель на 3 элемент массива
    
    group[0] = 5;
    *(iPointer) = 6;

    std::cout << "i: " << i << std::endl;
    std::cout << "number: " << number << std::endl;
    std::cout << "group[0]: " << group[0] << std::endl;
    std::cout << "group[2]: " << group[2] << std::endl;

    return 0;
}



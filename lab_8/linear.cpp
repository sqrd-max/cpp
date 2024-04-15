#include <iostream>
#include <vector>
#include <span>
#include <numeric>

// Определяем перечисление для указания порядка расположения элементов
enum class Order {
    RowMajor,
    ColumnMajor
};

// Определяем шаблонную структуру для хранения линейного массива
template<typename T>
struct Linear {
    std::vector<T> data; // Вектор для хранения данных массива
    std::vector<size_t> sizes; // Вектор для размеров измерений
    Order order; // Порядок расположения элементов

    // Конструктор для инициализации массива
    Linear(std::span<size_t> sizes, Order order) : sizes(sizes.begin(), sizes.end()), order(order) {
        size_t totalSize = std::accumulate(sizes.begin(), sizes.end(), 1, std::multiplies<size_t>());
        data.resize(totalSize);
    }
};

// Функция для получения элемента по многомерному индексу
template<typename T>
T* getElement(Linear<T>& array, std::span<size_t> indices) {
    if (indices.size() != array.sizes.size()) {
        return nullptr; // Ошибка: размерности не совпадают
    }
    size_t index = 0;
    size_t factor = 1;
    if (array.order == Order::RowMajor) {
        for (int i = indices.size() - 1; i >= 0; --i) {
            index += indices[i] * factor;
            factor *= array.sizes[i];
        }
    } else {
        for (size_t i = 0; i < indices.size(); ++i) {
            index += indices[i] * factor;
            factor *= array.sizes[i];
        }
    }
    return &array.data[index];
}

// Функция для доступа ко всем элементам массива
template<typename T>
std::span<T> elements(Linear<T>& array) {
    return std::span<T>(array.data);
}

int main() {
    std::vector<size_t> sizes = {3, 3, 3}; // Объявление трехмерного массива 3x3x3
    Linear<int> myArray(sizes, Order::RowMajor); // Создание линейного массива

    // Заполнение массива инкрементальными значениями
    for (int i = 0; i < myArray.data.size(); ++i) {
        myArray.data[i] = i;
    }

    // Получение элемента по индексу [1, 2, 2]
    std::vector<size_t> indices = {1, 2, 2};
    int* value = getElement(myArray, indices);
    if (value) {
        std::cout << "Element at [1,2,2]: " << *value << std::endl;
    }

    // Вывод всех элементов массива
    std::span<int> allElements = elements(myArray);
    std::cout << "All elements: ";
    for (auto& elem : allElements) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    return 0;
}

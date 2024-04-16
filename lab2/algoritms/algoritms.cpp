#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <random>
#include <span> 
#include <functional>

// Линейный поиск
template <typename T>
int linearSearch(const std::span<T>& arr, const T& target) {
    for (int i = 0; i < arr.size(); ++i) {
        if (arr[i] == target) {
            return i; // Найдено значение, возвращаем его индекс
        }
    }
    return -1; // Значение не найдено
}

// Бинарный поиск
int binarySearch(const std::span<int>& arr, int target) {
    int left = 0;
    int right = arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            return mid;
        }
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

// Интерполяционный поиск
int interpolationSearch(const std::span<int>& arr, int target) {
    int left = 0;
    int right = arr.size() - 1;
    while (left <= right && target >= arr[left] && target <= arr[right]) {
        int pos = left + ((target - arr[left]) * (right - left)) / (arr[right] - arr[left]);
        if (arr[pos] == target) {
            return pos;
        }
        if (arr[pos] < target) {
            left = pos + 1;
        } else {
            right = pos - 1;
        }
    }
    return -1;
}

// Функция для генерации случайного вектора
std::vector<int> generateRandomVector(int size) {
    std::vector<int> vec(size);
    std::iota(vec.begin(), vec.end(), 1); // Заполняем вектор значениями от 1 до size
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vec.begin(), vec.end(), g); // Перемешиваем вектор случайным образом
    return vec;
}

// Полиморфная функция для замера времени выполнения алгоритма поиска
template <typename SearchFunction>
void measureSearchTime(const std::vector<int>& sizes, int numTests, SearchFunction searchFunction, std::vector<double>& times) {
    std::random_device rd;
    std::mt19937 gen(rd());
    gen.seed(std::random_device{}()); 

    for (int i = 0; i < sizes.size(); ++i) {
        std::vector<int> sortedArray(sizes[i]);
        std::iota(sortedArray.begin(), sortedArray.end(), 1); 
        std::span<int> spanArray(sortedArray);
        double totalTime = 0;

        for (int test = 0; test < numTests; ++test) {
            int target = std::uniform_int_distribution<>(1, sizes[i])(gen);
            auto start = std::chrono::high_resolution_clock::now();
            searchFunction(spanArray, target); 
            auto end = std::chrono::high_resolution_clock::now();
            totalTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        }

        times[i] = totalTime / numTests; 
    }
}



int main() {
    const int numTests = 5;
    const int minSize = 1000;
    const int maxSize = 10000;
    const int step = 1000;
    std::vector<int> sizes;

    for (int size = minSize; size <= maxSize; size += step) {
        sizes.push_back(size);
    }

    std::vector<double> linearSearchTimes(sizes.size(), 0);
    std::vector<double> binarySearchTimes(sizes.size(), 0);
    std::vector<double> interpolationSearchTimes(sizes.size(), 0);

    // Замеры времени для линейного поиска
    measureSearchTime(sizes, numTests, linearSearch<int>, linearSearchTimes);

    // Замеры времени для бинарного поиска
    measureSearchTime(sizes, numTests, binarySearch, binarySearchTimes);

    // Замеры времени для интерполяционного поиска
    measureSearchTime(sizes, numTests, interpolationSearch, interpolationSearchTimes);


    // Вывод результатов
    std::cout << "Size\tLinear Search\tBinary Search\tInterpolation Search\n";
    for (int i = 0; i < sizes.size(); ++i) {
        std::cout << sizes[i] << "\t" << linearSearchTimes[i] << "\t\t" << binarySearchTimes[i] << "\t\t" << interpolationSearchTimes[i] << "\n";
    }

    return 0;
}

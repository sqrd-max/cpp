#include <iostream>
#include <vector>
#include <span>
#include <numeric>
#include <type_traits>

enum class Order {
    RowMajor,
    ColumnMajor
};

using SignedSize = std::make_signed_t<size_t>;

struct Range {
    SignedSize from;
    SignedSize toInclusive;
};

template<typename T>
struct LinearRanged {
    std::vector<T> data;
    std::vector<Range> ranges;
    std::vector<size_t> strides;
    size_t offset;
    Order order;

    // Конструктор для инициализации массива
    LinearRanged(std::span<Range> ranges, Order order)
        : ranges(ranges.begin(), ranges.end()), order(order), offset(0) {
        std::vector<size_t> sizes;
        strides.resize(ranges.size(), 1);

        for (auto& range : this->ranges) {
            sizes.push_back(static_cast<size_t>(range.toInclusive - range.from + 1));
        }

        if (order == Order::RowMajor) {
            for (size_t i = 1; i < sizes.size(); ++i) {
                strides[i] = strides[i - 1] * sizes[i - 1];
            }
        } else {
            for (size_t i = sizes.size() - 2; i < sizes.size(); --i) {
                strides[i] = strides[i + 1] * sizes[i + 1];
            }
        }

        offset = calculateOffset(this->ranges);
        data.resize(std::accumulate(sizes.begin(), sizes.end(), 1, std::multiplies<size_t>()));
    }

    size_t calculateOffset(std::span<Range> ranges) {
        size_t sum = 0;
        for (size_t i = 0; i < ranges.size(); ++i) {
            sum += ranges[i].from * (i > 0 ? strides[i] : 1);
        }
        return sum;
    }
};

template<typename T>
T* getElement(LinearRanged<T>& array, std::span<SignedSize> indices) {
    if (indices.size() != array.ranges.size()) {
        return nullptr; // Ошибка: размерности не совпадают
    }
    size_t index = 0;
    for (size_t i = 0; i < indices.size(); ++i) {
        if (indices[i] < array.ranges[i].from || indices[i] > array.ranges[i].toInclusive) {
            return nullptr; // Ошибка: индекс вне допустимого диапазона
        }
        index += (indices[i] - array.ranges[i].from) * array.strides[i];
    }
    index -= array.offset;
    return &array.data[index];
}

int main() {
    std::vector<Range> ranges = {{1, 3}, {1, 3}, {1, 3}}; // Интервалы индексов для каждого измерения
    LinearRanged<int> myArray(ranges, Order::RowMajor); // Создание линейного массива

    // Примеры индексов для доступа к элементам
    std::vector<SignedSize> indices = {1, 1, 1};
    int* value = getElement(myArray, indices);
    if (value) {
        std::cout << "Element at [1,1,1]: " << *value << std::endl;
    } else {
        std::cout << "Index out of range." << std::endl;
    }

    return 0;
}

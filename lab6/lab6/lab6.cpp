#include <iostream>
#include <vector>
#include <string_view>
#include <functional>

// Структура узла бакета, содержит ключ, значение и указатель на следующий узел в списке
struct BucketNode {
    std::string key; // ключ узла для сопоставления, используем string для удобства
    int value; // хранимое значение
    BucketNode* next = nullptr; // указатель на следующий узел в цепочке

    // Конструктор для инициализации нового узла
    BucketNode(std::string_view key, int value) : key(key), value(value), next(nullptr) {}
};

// Функциональный тип для хеш-функции, принимает ключ и возвращает хеш-код
using HashFunc = std::function<size_t(std::string_view)>;

// Структура хеш-таблицы
struct HashTable {
    std::vector<BucketNode*> buckets; // вектор указателей на узлы, представляющих бакеты
    HashFunc hashFunc; // хеш-функция

    // Конструктор для инициализации хеш-таблицы с заданным количеством бакетов и хеш-функцией
    HashTable(size_t capacity, HashFunc hashFunc) : buckets(capacity, nullptr), hashFunc(hashFunc) {}
};

// Вычисляет индекс бакета для ключа
size_t computeIndex(const HashTable& table, std::string_view key) {
    size_t hash = table.hashFunc(key); // получаем хеш-код ключа
    size_t index = hash % table.buckets.size(); // определяем индекс бакета как остаток от деления хеш-кода на количество бакетов
    return index;
}

// Поиск значения по ключу
int* find(HashTable& table, std::string_view key) {
    size_t index = computeIndex(table, key); // вычисляем индекс бакета для ключа
    BucketNode* currentNode = table.buckets[index]; // получаем начальный узел цепочки для бакета
    while (currentNode != nullptr) { // итерируем по узлам в цепочке
        if (currentNode->key == key) { // если ключ совпадает
            return &currentNode->value; // возвращаем указатель на значение
        }
        currentNode = currentNode->next; // переходим к следующему узлу в цепочке
    }
    return nullptr; // если ключ не найден, возвращаем nullptr
}

// Добавление нового значения с ключом в хеш-таблицу
int* add(HashTable& table, std::string_view key, int value) {
    size_t index = computeIndex(table, key); // вычисляем индекс бакета для ключа
    auto newNode = new BucketNode(key, value); // создаём новый узел с заданными ключом и значением
    newNode->next = table.buckets[index]; // вставляем новый узел в начало цепочки
    table.buckets[index] = newNode; // обновляем указатель на начало цепочки в бакете
    return &newNode->value; // возвращаем указатель на значение нового узла
}

// Удаление значения по ключу из хеш-таблицы
void remove(HashTable& table, std::string_view key) {
    size_t index = computeIndex(table, key); // вычисляем индекс бакета для ключа
    BucketNode *current = table.buckets[index], *prev = nullptr; // начинаем с начального узла цепочки
    while (current != nullptr) { // итерируем по узлам в цепочке
        if (current->key == key) { // если ключ совпадает
            if (prev != nullptr) { // если это не первый узел в цепочке
                prev->next = current->next; // удаляем текущий узел из цепочки
            } else {
                table.buckets[index] = current->next; // если это первый узел, обновляем начало цепочки
            }
            delete current; // освобождаем память узла
            break;
        }
        prev = current; // сохраняем предыдущий узел
        current = current->next; // переходим к следующему узлу
    }
}

// Освобождение памяти, выделенной для хеш-таблицы
void freeHashTable(HashTable& table) {
    for (auto& bucket : table.buckets) { // итерируем по всем бакетам
        BucketNode* currentNode = bucket;
        while (currentNode != nullptr) { // итерируем по узлам в каждом бакете
            BucketNode* nextNode = currentNode->next;
            delete currentNode; // удаляем текущий узел
            currentNode = nextNode; // переходим к следующему узлу
        }
    }
}

int main() {
    // Пример использования
    HashFunc hashFunc = [](std::string_view key) -> size_t {
        size_t hash = 0;
        for (char ch : key) {
            hash = hash * 31 + ch; // простой хеш-функции на основе умножения и сложения
        }
        return hash;
    };

    HashTable table(10, hashFunc); // создаем хеш-таблицу с 10 бакетами

    add(table, "key1", 1); // добавляем элементы
    add(table, "key2", 2);

    if (int* value = find(table, "key1")) { // поиск элемента по ключу
        std::cout << "Found key1: " << *value << std::endl;
    }

    remove(table, "key1"); // удаляем элемент

    if (int* value = find(table, "key1")) { // проверяем, что элемент действительно удален
        std::cout << "Found key1 after removal: " << *value << std::endl;
    } else {
        std::cout << "key1 not found after removal" << std::endl;
    }

    freeHashTable(table); // освобождаем ресурсы

    return 0;
}

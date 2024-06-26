#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>

template <typename T>
struct BucketNode {
    T value;
    std::string key;
    std::unique_ptr<BucketNode<T>> next;

    BucketNode(T val, std::string keyVal) : value(std::move(val)), key(std::move(keyVal)), next(nullptr) {}
};

template <typename T>
using HashFunc = std::function<size_t(std::string_view)>;

template <typename T>
struct HashTable {
    std::vector<std::unique_ptr<BucketNode<T>>> buckets;
    HashFunc<T> hashFunc;

    explicit HashTable(size_t size, HashFunc<T> func) : buckets(size), hashFunc(func) {}
};

template <typename T>
size_t computeIndex(const HashTable<T>& table, std::string_view key) {
    size_t hash = table.hashFunc(key);
    size_t index = hash % table.buckets.size();
    return index;
}

template <typename T>
T* add(HashTable<T>& table, std::string_view key, const T& value) {
    size_t index = computeIndex(table, key);
    auto newNode = std::make_unique<BucketNode<T>>(value, std::string(key));
    if (table.buckets[index]) {
        newNode->next = std::move(table.buckets[index]);
    }
    table.buckets[index] = std::move(newNode);
    return &(table.buckets[index]->value);
}

template <typename T>
T* find(HashTable<T>& table, std::string_view key) {
    size_t index = computeIndex(table, key);
    BucketNode<T>* node = table.buckets[index].get();
    while (node) {
        if (node->key == key) {
            return &node->value;
        }
        node = node->next.get();
    }
    return nullptr;
}

template <typename T>
void remove(HashTable<T>& table, std::string_view key) {
    size_t index = computeIndex(table, key);
    auto& bucket = table.buckets[index];
    if (!bucket) return;

    if (bucket->key == key) {
        bucket = std::move(bucket->next);
        return;
    }

    BucketNode<T>* current = bucket.get();
    while (current && current->next) {
        if (current->next->key == key) {
            current->next = std::move(current->next->next);
            return;
        }
        current = current->next.get();
    }
}

template <typename T>
void freeHashTable(HashTable<T>& table) {
    for (auto& bucket : table.buckets) {
        bucket.reset();
    }
    table.buckets.clear();
}

int main() {
    HashFunc<int> hashFunc = [](std::string_view key) -> size_t {
        size_t hash = 0;
        for (char ch : key) {
            hash = hash * 31 + ch; // простой хеш-функции на основе умножения и сложения
        }
        return hash;
    };

    HashTable<int> table(10, hashFunc);

    add(table, "key1", 1); 
    add(table, "key2", 2);

    if (int* value = find(table, "key1")) { 
        std::cout << "Found key1: " << *value << std::endl;
    }

    remove(table, "key1"); // удаляем элемент

    if (int* value = find(table, "key1")) { 
        std::cout << "Found key1 after removal: " << *value << std::endl;
    } else {
        std::cout << "key1 not found after removal" << std::endl;
    }

    freeHashTable(table); 

    return 0;
}

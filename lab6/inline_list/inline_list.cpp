#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <span>

template <typename T>
struct Bucket {
    bool occupied;
    std::string key;
    T value;
};

template <typename T>
using HashFunc = std::function<size_t(std::string_view)>;

template <typename T>
struct HashTable {
    std::vector<Bucket<T>> buckets;
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
    size_t startIndex = index;

    do {
        Bucket<T>& bucket = table.buckets[index];
        if (!bucket.occupied) {
            bucket.occupied = true;
            bucket.key = std::string(key);
            bucket.value = value;
            return &bucket.value;
        }
        index = (index + 1) % table.buckets.size(); // Linear probing
    } while (index != startIndex);

    // Hash table is full
    return nullptr;
}

template <typename T>
T* find(HashTable<T>& table, std::string_view key) {
    size_t index = computeIndex(table, key);
    size_t startIndex = index;

    do {
        Bucket<T>& bucket = table.buckets[index];
        if (bucket.occupied && bucket.key == key) {
            return &bucket.value;
        }
        index = (index + 1) % table.buckets.size(); // Linear probing
    } while (index != startIndex);

    // Element not found
    return nullptr;
}


template <typename T>
void remove(HashTable<T>& table, std::string_view key) {
    size_t index = computeIndex(table, key);
    size_t startIndex = index;

    do {
        Bucket<T>& bucket = table.buckets[index];
        if (bucket.occupied && bucket.key == key) {
            size_t shiftIndex = index;
            while (true) {
                size_t nextIndex = (shiftIndex + 1) % table.buckets.size();
                if (!table.buckets[nextIndex].occupied || nextIndex == startIndex) {
                    break;
                }
                table.buckets[shiftIndex] = table.buckets[nextIndex];
                shiftIndex = nextIndex;
            }
            table.buckets[shiftIndex].occupied = false;  
            return;
        }
        index = (index + 1) % table.buckets.size(); 
    } while (index != startIndex);

}


template <typename T>
void freeHashTable(HashTable<T>& table) {
    table.buckets.clear();
}

int main() {
    HashFunc<int> hashFunc = [](std::string_view key) -> size_t {
        size_t hash = 0;
        for (char ch : key) {
            hash = hash * 31 + ch; 
        }
        return hash;
    };

    HashTable<int> table(10, hashFunc); 

    add(table, "key1", 1); 
    add(table, "key2", 2);

    if (int* value = find(table, "key1")) { 
        std::cout << "Found key1: " << *value << std::endl;
    }

    remove(table, "key1"); 

    if (int* value = find(table, "key1")) {
        std::cout << "Found key1 after removal: " << *value << std::endl;
    } else {
        std::cout << "key1 not found after removal" << std::endl;
    }

    freeHashTable(table); 

    return 0;
}

#pragma once
#include <iostream>
#include <unordered_map>
#include <list>
#include <fstream>

using namespace std;

template<typename Key, typename T>
class LRUCache {
private:
    size_t capacity;
    list<pair<Key, T>> itemList;
    unordered_map<Key, typename list<pair<Key, T>>::iterator> itemMap;

    void writeToDisk(const Key& key, const T& data) {
        ofstream out("lru_spill.bin", ios::app | ios::binary);
        out.write(reinterpret_cast<const char*>(&key), sizeof(Key));
        out.write(reinterpret_cast<const char*>(&data), sizeof(T));
        out.close();
    }

public:
    LRUCache(size_t cap) : capacity(cap) {}

    void put(const Key& key, const T& value) {
        if (itemMap.find(key) != itemMap.end()) {
            itemList.erase(itemMap[key]);
        } else if (itemList.size() >= capacity) {
            auto last = itemList.back();
            writeToDisk(last.first, last.second); // simulate eviction
            itemMap.erase(last.first);
            itemList.pop_back();
        }

        itemList.push_front({key, value});
        itemMap[key] = itemList.begin();
    }

    T* get(const Key& key) {
        if (itemMap.find(key) == itemMap.end()) return nullptr;
        itemList.splice(itemList.begin(), itemList, itemMap[key]);
        return &itemMap[key]->second;
    }
};

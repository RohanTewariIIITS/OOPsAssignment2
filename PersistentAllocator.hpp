// PersistentAllocator.hpp

#ifndef PERSISTENT_ALLOCATOR_HPP
#define PERSISTENT_ALLOCATOR_HPP

#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

class PersistentMemory {
public:
    static void storeToFile(const void* data, size_t size, const string& filename) {
        ofstream out(filename, ios::binary);
        out.write(reinterpret_cast<const char*>(data), size);
        out.close();
    }

    static void loadFromFile(void* data, size_t size, const string& filename) {
        ifstream in(filename, ios::binary);
        in.read(reinterpret_cast<char*>(data), size);
        in.close();
    }
};

template <typename T>
class PersistentAllocator {
public:
    T* allocate(size_t n) {
        if (n > availableMemory()) {
            cout << "[INFO] Not enough RAM. Spilling objects to SSD/NVM...\n";
            T* obj = reinterpret_cast<T*>(malloc(n * sizeof(T)));
            PersistentMemory::storeToFile(obj, n * sizeof(T), "spill_data.bin");
            return obj;
        }

        return new T[n];
    }

    void deallocate(T* ptr, size_t n) {
        if (ptr == nullptr) return;
        cout << "[INFO] Deallocating memory...\n";
        free(ptr);
    }

    void* operator new(size_t size) {
        return malloc(size);
    }

    void operator delete(void* pointer) {
        free(pointer);
    }

    size_t availableMemory() {
        return 1024 * 1024; // 1MB
    }
};

#endif // PERSISTENT_ALLOCATOR_HPP

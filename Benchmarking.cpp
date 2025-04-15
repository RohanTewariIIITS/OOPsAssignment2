#include "LRUCache.hpp"
#include "chrono"
#include "PersistentAllocator.hpp"

class MyObject {
public:
    int data;
    MyObject(int d = 0) : data(d) {}
    void print() const { std::cout << "Data: " << data << std::endl; }
};

struct TestObject {
    int id;
    double value;
};

int main() {
    size_t objectCount = 100000;
    size_t cacheCapacity = 10000;

    std::cout << "\n=== Benchmark: Traditional LRU Cache ===\n";
    LRUCache<int, MyObject> cache(cacheCapacity);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < objectCount; ++i) {
        cache.put(i, MyObject(i));
    }
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "LRU cache insert + spill time: " << duration.count() << " ms\n";

    const size_t numObjects = 200000; // Try to exceed 1MB
    PersistentAllocator<TestObject> allocator;

    cout << "\n\n=== Benchmark: Starting allocation of " << numObjects << " TestObject(s)===\n";

    auto startPer = chrono::high_resolution_clock::now();

    TestObject* objects = allocator.allocate(numObjects);
    for (size_t i = 0; i < numObjects; ++i) {
        objects[i].id = i;
        objects[i].value = i * 1.5;
    }

    auto endPer = chrono::high_resolution_clock::now();
    auto durationPers = chrono::duration_cast<chrono::milliseconds>(endPer - startPer).count();

    cout << "Allocation + persistence time: "<< durationPers << " ms\n";

    allocator.deallocate(objects, numObjects);
    return 0;
}

// main.cpp
#include<iostream>
#include "PersistentAllocator.hpp"

using namespace std;

class MyObject {
public:
    int data;

    MyObject(int d = 0) : data(d) {}

    void print() {
        cout << "Data: " << data << endl;
    }
};

int main() {
    PersistentAllocator<MyObject> allocator;

    size_t objectCount = 100000;

    MyObject* objArray = allocator.allocate(objectCount);

    for (size_t i = 0; i < objectCount; ++i) {
        objArray[i] = MyObject(i);
    }

    for (size_t i = 0; i < 5; ++i) {
        objArray[i].print();
    }

    allocator.deallocate(objArray, objectCount);

    return 0;
}

#include "MemoryPool.h"
#include <cstdlib>
#include <iostream>

using namespace std;

MemoryPool::MemoryPool(size_t objectSize, size_t poolSize)
    : objectSize(objectSize), poolSize(poolSize) {
    poolMemory = std::malloc(objectSize * poolSize);
    if (!poolMemory) {
        throw std::bad_alloc();
    }

    // Initialize free list
    for (size_t i = 0; i < poolSize; ++i) {
        void* ptr = static_cast<char*>(poolMemory) + i * objectSize;
        freeList.push_back(ptr);
    }
}

MemoryPool::~MemoryPool() {
    std::free(poolMemory);
}

void* MemoryPool::allocate() {
    totalAllocated += objectSize;
    std::lock_guard<std::mutex> lock(poolMutex);
    if (freeList.empty()) {
        std::cerr << "MemoryPool: Out of memory!" << std::endl;
        throw std::bad_alloc();
    }
    void* ptr = freeList.back();
    freeList.pop_back();
    std::cout << "MemoryPool: Allocated object at " << ptr << std::endl;
    return ptr;
}

void MemoryPool::deallocate(void* ptr) {
    totalDeallocated += objectSize;
    std::lock_guard<std::mutex> lock(poolMutex);
    freeList.push_back(ptr);
    std::cout << "MemoryPool: Deallocated object at " << ptr << std::endl;
}
void MemoryPool::reportMemoryUsage() {
    std::lock_guard<std::mutex> lock(poolMutex);
    std::cout << "MemoryPool Report:\n";
    std::cout << "Total Allocated: " << totalAllocated << " bytes\n";
    std::cout << "Total Deallocated: " << totalDeallocated << " bytes\n";
    std::cout << "Net Memory In Use: " << (totalAllocated - totalDeallocated) << " bytes\n";
    std::cout << "Free Slots: " << freeList.size() << "/" << poolSize << std::endl;
}
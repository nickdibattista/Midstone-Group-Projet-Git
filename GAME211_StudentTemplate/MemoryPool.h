#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include <cstddef>
#include <vector>
#include <mutex>

class MemoryPool {
public:
    MemoryPool(size_t objectSize, size_t poolSize);
    ~MemoryPool();

    void* allocate();
    void deallocate(void* ptr);
    size_t totalAllocated = 0;
    size_t totalDeallocated = 0;
    void reportMemoryUsage();
private:
    size_t objectSize;
    size_t poolSize;
    std::vector<void*> freeList;
    void* poolMemory;
    std::mutex poolMutex; // For thread safety
};

#endif

#ifndef _MEMORY_ALLOCATOR_HPP
#define _MEMORY_ALLOCATOR_HPP

#include "../lib/hw.h"

#define MEMORY_POOL_SIZE = HEAP_START_ADDR - HEAP_END_ADDR // veličina memorijskog prostora

typedef struct FreeBlock {
    size_t size;               // veličina slobodnog bloka
    FreeBlock* next;          // pokazivač na sledeći slobodni blok
} FreeBlock;

static FreeBlock* freeBlockHead;


class MemoryAllocator{
public:
    static MemoryAllocator& getInstance(); //vraca nam jedinu instancu klase

    void* allocate(size_t size);

    int deallocate(void* p);

    bool isValidPointer(uint64* ptr);

private:
    MemoryAllocator() { // privatni konstruktor   // gde ovu inicijalizaciju odraditi
        size_t heapSize = (size_t)((char*)HEAP_START_ADDR -(char*)HEAP_END_ADDR);
        freeBlockHead = (FreeBlock*)((uint8*)HEAP_START_ADDR);
        freeBlockHead->size = heapSize - sizeof(FreeBlock);
        freeBlockHead->next = nullptr;
    };
    MemoryAllocator(const MemoryAllocator&) = delete; // onemoguceno kopiranje
    MemoryAllocator& operator=(const MemoryAllocator&) = delete; // onemoguceno dodeljivanje

};


#endif
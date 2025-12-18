#ifndef _SEM_HPP
#define _SEM_HPP
#include "../h/Queue.hpp"
#include "../lib/mem.h"
#include "../lib/hw.h"
#include "../h/pcb.hpp"
#include "../h/memallocator.hpp"

class SemaphoreAbstraction {
public:

    ~SemaphoreAbstraction();
    SemaphoreAbstraction(int init);
    int wait();
    int signal();
    int closeSem();
    int trywait();

    static SemaphoreAbstraction *createSemaphore(int init);

    void* operator new(size_t size){ return MemoryAllocator::getInstance().allocate(size); }
    void operator delete(void* p){ MemoryAllocator::getInstance().deallocate(p); }
    void* operator new[](size_t size){ return MemoryAllocator::getInstance().allocate(size); }
    void operator delete[](void* p){ MemoryAllocator::getInstance().deallocate(p); }

    uint64 getValue();
    bool isSemClosed();


private:
    int value; //int
    Queue blocked;
    void block();
    void unblock();
    bool semClosed;


};

#endif
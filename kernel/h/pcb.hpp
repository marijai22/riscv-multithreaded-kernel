#ifndef _PCB_HPP
#define _PCB_HPP
#include "../lib/hw.h"
#include "../lib/mem.h"
#include "../h/scheduler.hpp"
#include "../h/syscall_c.h"
#include "../h/memallocator.hpp"

class PCB{
public:

    ~PCB() {
        delete[] stack;}

    enum ThreadStates {
        INACTIVE,
        READY,
        RUNNING,
        BLOCKED,
        TERMINATED
    };


    void* operator new(size_t size){ return MemoryAllocator::getInstance().allocate(size); }
    void operator delete(void* p){ MemoryAllocator::getInstance().deallocate(p); }
    void* operator new[](size_t size){ return MemoryAllocator::getInstance().allocate(size); }
    void operator delete[](void* p){ MemoryAllocator::getInstance().deallocate(p); }

    uint64 getTimeSlice() const{
        return timeSlice;
    }

    bool isFinished() const { return state == ThreadStates::TERMINATED; }

    void setStatus(ThreadStates status) { state = status; }

    void setKernel(){kernelThread = true;}

    uint64 getId() {return id;}

    using Body = void (*)(void*);
    static PCB *running;

    static uint64 threadIdleId;
    static PCB *createThread(Body body, void* arg, uint64 * stack, bool kernelThread);


    static int exit();

private:
    struct Context{
        uint64 ra;
        uint64 sp;
    };


    explicit PCB(Body body, void* arg, uint64 timeSlice, uint64 * stack, bool kernel): body(body),arg(arg),
        stack(body != nullptr ? stack : nullptr),
        context({
                (uint64) &threadWrapper,  //da odmah krene od izvrsavanja tela korutine
                stack != nullptr ? ((uint64) &stack[2*DEFAULT_STACK_SIZE]): 0
                }), timeSlice(timeSlice), id(++identificator), state(ThreadStates::INACTIVE),
                kernelThread(kernel)
    {
        if (body != nullptr) {
            state = ThreadStates::READY;
            Scheduler::put(this);
        }
    };

    Body body;
    void* arg;
    uint64 *stack;
    Context context;
    uint64 timeSlice;
    uint64 id;
    ThreadStates state;
    bool kernelThread;

    static uint64 identificator;

    static uint64 timeSliceCounter;

    friend class Riscv;
    friend class SemaphoreAbstraction;

    static void dispatch();

    static void threadWrapper();

    static void idleThread(void*);

    static void contextSwitch(Context *oldContext, Context *runningContext);

};

#endif
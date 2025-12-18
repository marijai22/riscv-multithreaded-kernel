#include "../lib/hw.h"
#include "../h/memallocator.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../h/syscall_cpp.hpp"
#include "../h/pcb.hpp"

#include "../test/printing.hpp"
#include "../h/userMain.hpp"


void userWrapper(void*)
{
    printString("userMain started!\n");
    userMain();
    printString("userMain finished!\n");
}


void main(){

    thread_t mainThread=PCB::createThread(nullptr, nullptr, nullptr, true);
    PCB::running=mainThread;

    Riscv::w_stvec((uint64)&Riscv::supervisorTrap);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);


    thread_t userThread=PCB::createThread(&userWrapper, nullptr, (uint64*)MemoryAllocator::getInstance().allocate(DEFAULT_STACK_SIZE*sizeof(uint64)), false);

    while(!userThread->isFinished()){
        thread_dispatch();

    }

    delete userThread;

    return;
}

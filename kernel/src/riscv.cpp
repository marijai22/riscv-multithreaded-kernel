#include "../h/riscv.hpp"
#include "../h/memallocator.hpp"
#include "../h/pcb.hpp"
#include "../lib/console.h"
#include "../test/printing.hpp"
#include "../h/sem.hpp"
#include "../h/sem.hpp"

void Riscv::firstTimeChosen(){

    uint64 volatile sstatus;
    sstatus = r_sstatus();
    if(PCB::running->kernelThread){
        sstatus = sstatus | SSTATUS_SPP;
    }
    else{
        sstatus = sstatus & ~SSTATUS_SPP;
    }
    w_sstatus(sstatus);
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap() {

    uint64 scause = r_scause();

    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) { //ecall iz korisnickog rezima ili privilegovanog(ako neko pozove mem_alloc)
        uint64 a0 = r_a0();
        PCB::timeSliceCounter = 0;
        switch (a0) {
            case 0x01: {
                size_t size = r_a1();
                size = size*MEM_BLOCK_SIZE;
                void *addr = MemoryAllocator::getInstance().allocate(size);
                if (addr != nullptr) {
                    __asm__ volatile("mv a0, %[allocAddress]" : : [allocAddress] "r"((uint8 *)addr));
                } else {
                    __asm__ volatile("li a0, 0");
                }
                uint64 sepc = r_sepc() + 4;
                w_sepc(sepc);
                break;
            }
            case 0x02: {
                void *pointer = (void *) r_a1();
                int resultVal = MemoryAllocator::getInstance().deallocate(pointer);
                __asm__ volatile("mv a0, %[allocAddress]" : : [allocAddress] "r"(resultVal));
                uint64 sepc = r_sepc() + 4;
                w_sepc(sepc);
                break;
            }
            case 0x11: {
                int retVal;
                thread_t* handle = (thread_t *) r_a1();
                PCB::Body body = (PCB::Body)r_a2();
                void* args = (void*)r_a6();
                uint64 * stack = (uint64 *)r_a7();
                *handle = (thread_t)PCB::createThread(body, args, stack, false);
                if(!handle){retVal = -1;}
                else{retVal = 0;}
                __asm__ volatile("mv a0, %[rV]" : : [rV] "r"(retVal));
                uint64 sepc = r_sepc() + 4;
                w_sepc(sepc);
                break;
            }
            case 0x12:{
                int result = 0;
                PCB::running->setStatus(PCB::ThreadStates::TERMINATED);
                uint64 volatile sepc = r_sepc() + 4;
                uint64 volatile sstatus = r_sstatus();
                PCB::dispatch();
                __asm__ volatile("mv a0, %[val]" : : [val] "r"(result));
                w_sstatus(sstatus);
                w_sepc(sepc);
                break;
            }
            case 0x13: {
                uint64 volatile sepc = r_sepc() + 4;
                uint64 volatile sstatus = r_sstatus();
                PCB::dispatch();
                w_sstatus(sstatus);
                w_sepc(sepc);
                break;
            }
            case 0x21: {
                int retVal = 0;
                sem_t* handle = (sem_t *) r_a1();
                if(!handle){retVal = -1;}
                unsigned init = (unsigned)r_a2();
                *handle = (sem_t)SemaphoreAbstraction::createSemaphore(init);
                if(!handle){retVal = -1;}
                __asm__ volatile("mv a0, %[val]" : : [val] "r"(retVal));
                uint64 volatile sepc = r_sepc()+4;
                w_sepc(sepc);
                break;
            }
            case 0x22:{
                int retVal = 0;
                sem_t handle = (sem_t) r_a1();
                if(!handle){retVal = -1;}
                handle->~SemaphoreAbstraction();
                __asm__ volatile("mv a0, %[val]" : : [val] "r"(retVal));
                uint64 volatile sepc = r_sepc()+4;
                w_sepc(sepc);
                break;
            }
            case 0x23:{
                int retVal;
                sem_t id = (sem_t) r_a1();
                if(!id){retVal = -1;}
                uint64 volatile sepc = r_sepc() + 4;
                uint64 volatile sstatus = r_sstatus();
                retVal = id->wait();
                w_sstatus(sstatus);
                w_sepc(sepc);
                __asm__ volatile("mv a0, %[val]" : : [val] "r"(retVal));
                break;
            }
            case 0x24:{
                int retVal;
                sem_t id = (sem_t) r_a1();
                if(!id){retVal = -1;}
                retVal = id->signal();
                __asm__ volatile("mv a0, %[val]" : : [val] "r"(retVal));
                uint64 volatile sepc = r_sepc()+4;
                w_sepc(sepc);
                break;
            }
            case 0x26:{
                int retVal;
                sem_t id = (sem_t) r_a1();
                if(!id){retVal = -1;}
                retVal = id->trywait();
                __asm__ volatile("mv a0, %[val]" : : [val] "r"(retVal));
                uint64 volatile sepc = r_sepc()+4;
                w_sepc(sepc);
                break;
            }
            case(0x41):{
                uint64 volatile sepc = r_sepc()+4;
                char a=__getc();
                __asm__ volatile("mv a0, %0" : : "r" (a));
                w_sepc(sepc);
                break;
            }
            case(0x42):{
                uint64 c = r_a1();
                uint64 volatile sepc = r_sepc()+4;
                __putc(c);
                w_sepc(sepc);
                break;
            }
        }


    }else if(scause == 0x8000000000000001UL)//BNT = 1, exception:no
    {
        /*
         PCB::timeSliceCounter++;
         if (PCB::timeSliceCounter >= PCB::running->getTimeSlice())
         {
             uint64 volatile sepc = r_sepc();
             uint64 volatile sstatus = r_sstatus();
             PCB::timeSliceCounter = 0;
             PCB::dispatch();
             sstatus = previousPrivilege(sstatus);
             w_sstatus(sstatus);
             w_sepc(sepc);
         }*/
        mc_sip(SIP_SSIP);

    }else if(scause == 0x8000000000000009UL){

        console_handler();
    }else{
        uint64 sepc = r_sepc();
        uint64 stval = r_stval();

        printString("scause: ");
        printInt(scause);
        printString("\n");
        printString("sepc: ");
        printInt(sepc);
        printString("\n");
        printString("stval: ");
        printInt(stval);
    }
}

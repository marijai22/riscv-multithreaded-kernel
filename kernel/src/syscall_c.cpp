#include "../h/syscall_c.h"
#include "../lib/hw.h"
#include "../h/memallocator.hpp"
#include "../lib/mem.h"
#include "../lib/console.h"

void* mem_alloc (size_t size){

    size_t sizeInBlocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    uint64 code = 0x01;
    void* allocateAddress;

    __asm__ volatile("mv a1, %[numofblck]" : : [numofblck] "r" (sizeInBlocks));
    __asm__ volatile("mv a0, %[instrcode]" : : [instrcode] "r" (code));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[address], a0" :  [address] "=r" (allocateAddress));

    return allocateAddress;

}


int mem_free (void* p){
    uint64 code = 0x02;
    int retValue;
    __asm__ volatile("mv a1, %[pointAddr]" : : [pointAddr] "r" ((uint64)p));
    __asm__ volatile("mv a0, %[instrcode]" : : [instrcode] "r" (code));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[val], a0" :  [val] "=r" (retValue));

    return retValue;
}

int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg){
    uint64 code = 0x11;
    int retValue;
    if(!handle) return -1;
    uint64 * stack = (uint64 *)mem_alloc(2*DEFAULT_STACK_SIZE* sizeof(uint64));
    if(stack == nullptr) {return -1;}
    __asm__ volatile("mv a7, %[stackp]" : : [stackp] "r" ((uint64)stack));
    __asm__ volatile("mv a6, %[params]" : : [params] "r" ((uint64)arg));
    __asm__ volatile("mv a2, %[startRout]" : : [startRout] "r" ((uint64)start_routine));
    __asm__ volatile("mv a1, %[handleNum]" : : [handleNum] "r" ((uint64)handle));
    __asm__ volatile("mv a0, %[instrcode]" : : [instrcode] "r" (code));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[val], a0" :  [val] "=r" (retValue));
    return retValue;
}

int thread_exit (){
    uint64 code = 0x12;
    int retValue;
    __asm__ volatile("mv a0, %[instrcode]" : : [instrcode] "r" (code));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[val], a0" :  [val] "=r" (retValue));
    return retValue;
}

void thread_dispatch (){
    uint64 code = 0x13;
    __asm__ volatile("mv a0, %[instrcode]" : : [instrcode] "r" (code));
    __asm__ volatile("ecall");
}

int time_sleep(time_t){
    return 0;
}

int sem_open (sem_t* handle, unsigned init){
    uint64 code = 0x21;
    int retValue;
    if(!handle) return -1;
    __asm__ volatile("mv a2, %[initValue]" : : [initValue] "r" ((uint64 )init));
    __asm__ volatile("mv a1, %[handleNum]" : : [handleNum] "r" ((uint64)handle));
    __asm__ volatile("mv a0, %[instrcode]" : : [instrcode] "r" (code));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[val], a0" :  [val] "=r" (retValue));
    return retValue;
}

int sem_close (sem_t handle){
    uint64 code = 0x22;
    int retValue;
    if(!handle) return -1;
    __asm__ volatile("mv a1, %[handleNum]" : : [handleNum] "r" ((uint64)handle));
    __asm__ volatile("mv a0, %[instrcode]" : : [instrcode] "r" (code));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[val], a0" :  [val] "=r" (retValue));
    return retValue;
}

int sem_wait (sem_t id){
    uint64 code = 0x23;
    int retValue;
    if(!id) return -1;
    __asm__ volatile("mv a1, %[idValue]" : : [idValue] "r" ((uint64)id));
    __asm__ volatile("mv a0, %[instrcode]" : : [instrcode] "r" (code));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[val], a0" :  [val] "=r" (retValue));
    return retValue;
}

int sem_signal (sem_t id){
    uint64 code = 0x24;
    int retValue;
    if(!id) return -1;
    __asm__ volatile("mv a1, %[idValue]" : : [idValue] "r" ((uint64)id));
    __asm__ volatile("mv a0, %[instrcode]" : : [instrcode] "r" (code));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[val], a0" :  [val] "=r" (retValue));
    return retValue;
}

int sem_trywait(sem_t id){
    uint64 code = 0x26;
    int retValue;
    if(!id) return -1;
    __asm__ volatile("mv a1, %[idValue]" : : [idValue] "r" ((uint64)id));
    __asm__ volatile("mv a0, %[instrcode]" : : [instrcode] "r" (code));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[val], a0" :  [val] "=r" (retValue));
    return retValue;
}

char getc(){

    uint64 code=0x41;
    __asm__ volatile("mv a0, %[instrcode]" : : [instrcode] "r" (code));
    char ret;
    __asm__ volatile("ecall");
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;

}

void putc(char c){
    uint64 code=0x42;
    __asm__ volatile("mv a1, %[val]" : : [val] "r" ((uint64)c));
    __asm__ volatile("mv a0, %[instrcode]" : : [instrcode] "r" (code));
    __asm__ volatile("ecall");
}
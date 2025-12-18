#include "../h/pcb.hpp"
#include "../h/riscv.hpp"



PCB *PCB::running = nullptr;
uint64 PCB::timeSliceCounter = 0;
uint64 PCB::threadIdleId = 0;


PCB* PCB::createThread(Body body, void* arg, uint64 * stack, bool kernelThread){
    return new PCB(body, arg, DEFAULT_TIME_SLICE, stack, kernelThread);
}


uint64 PCB::identificator = 0;


void PCB::threadWrapper(){
    Riscv::firstTimeChosen();
    running->body(running->arg);
   thread_exit();
}


void PCB::idleThread(void* p){
    while(1){};
}

/*
int PCB::exit() {
    if(running->state != ThreadStates::RUNNING) return -1;
    else{
        running->setStatus(ThreadStates::TERMINATED);
        dispatch();
        return 0;
    }
}
*/



void PCB::dispatch(){

    PCB* old = running;
    if (old->state!=ThreadStates::TERMINATED && old->state!=ThreadStates::BLOCKED) { old->state = ThreadStates::READY, Scheduler::put(old); }

    if(Scheduler::getSize() == 0 && threadIdleId==0){ //IDLE NIT
        PCB* threadIdle = createThread(idleThread, nullptr, new uint64 [DEFAULT_STACK_SIZE],false);
        threadIdleId = threadIdle->getId();
    }

    running = Scheduler::get();
    running->state = ThreadStates::RUNNING;

    PCB::contextSwitch(&old->context, &running->context);
}

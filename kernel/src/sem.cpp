#include "../h/sem.hpp"
#include "../h/pcb.hpp"

SemaphoreAbstraction* SemaphoreAbstraction::createSemaphore(int init){
    return new SemaphoreAbstraction(init);
}

SemaphoreAbstraction::SemaphoreAbstraction(int init) {
    value = init;
    semClosed = false;
}

SemaphoreAbstraction::~SemaphoreAbstraction(){
    closeSem();
}


uint64 SemaphoreAbstraction::getValue() {
    return  value;
}

bool SemaphoreAbstraction::isSemClosed(){
    return semClosed == true;
}

int SemaphoreAbstraction::wait() {
    if(isSemClosed()) return -1;
    if(--value < 0) {
        block();
        if(isSemClosed()) {return -1;}
    }
    return 0;
}

void SemaphoreAbstraction::block() {
    if(PCB::running->state == PCB::ThreadStates::RUNNING) {
        PCB::running->setStatus(PCB::ThreadStates::BLOCKED);
        blocked.put(PCB::running);
        //PCB::dispatch(); // tu negde ja mislim
        PCB* old=PCB::running;
         PCB::running=Scheduler::get();
         PCB::running->state = PCB::ThreadStates::RUNNING;
         PCB::contextSwitch(&old->context,&PCB::running->context);
    }else{return;}
}


int SemaphoreAbstraction::signal() {
    if(isSemClosed()) return -1;
    if(++value <=0) { unblock();}
    return 0;
}

void SemaphoreAbstraction::unblock() {
    PCB* unblocked = blocked.get();
    if(unblocked->state == PCB::ThreadStates::BLOCKED)
    {
        unblocked->setStatus(PCB::ThreadStates::READY);
        Scheduler::put(unblocked);
    }else{
        return;
    }
}


int SemaphoreAbstraction::closeSem() {
    if(isSemClosed()) return -1;
    semClosed = true;
    for(int i =0; i< blocked.getSize(); i++){
        unblock();
    }
    value = 0;
    return 0;
}

int SemaphoreAbstraction::trywait() {
    if(isSemClosed()) return -1;
    if(value-1 < 0) {return 1;}
    else { --value; return 0;}
}
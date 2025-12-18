#include "../h/scheduler.hpp"
#include "../h/pcb.hpp"

List<PCB> Scheduler::readyList;

void Scheduler::put(PCB* p){
    readyList.addLast(p);
}

PCB* Scheduler::get(){
    //IDLE NIT
    if(PCB::threadIdleId > 0 && Scheduler::getSize()>1){
        if(readyList.peekFirst()->getId() != PCB::threadIdleId){
            return readyList.removeFirst();
        }else{
            PCB* temp = readyList.removeFirst();
            readyList.addLast(temp);
        }
    }
    return readyList.removeFirst();
}

int Scheduler::getSize(){
    return readyList.getSize();
}

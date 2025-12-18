#include "../h/Queue.hpp"
#include "../h/pcb.hpp"


void Queue::put(PCB* p){
    blockedList.addLast(p);
}

PCB* Queue::get(){
    return blockedList.removeFirst();
}

int Queue::getSize(){
    return blockedList.getSize();
}
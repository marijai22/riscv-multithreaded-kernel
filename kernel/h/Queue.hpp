#ifndef _QUEUE_HPP
#define _QUEUE_HPP
#include "../h/list.hpp"


class PCB;

class Queue{
public:
    void put(PCB* p);

    PCB* get();

    int getSize();
private:
    List<PCB> blockedList;
};

#endif
#ifndef _SCHEDULER_HPP
#define _SCHEDULER_HPP
#include "../h/list.hpp"

class PCB;

class Scheduler{
public:

    static void put(PCB* p);

    static PCB* get();

    static int getSize();



private:
    static List<PCB> readyList;
};

#endif
#ifndef _SYSCALL_C_HPP
#define _SYSCALL_C_HPP
#include "../lib/hw.h"

const int EOF = -1;
//class _thread;
class PCB;
typedef PCB* thread_t;
//typedef _thread* thread_t;
class SemaphoreAbstraction;
typedef SemaphoreAbstraction* sem_t;
// _sem;
//typedef _sem* sem_t;
typedef unsigned long time_t;


void* mem_alloc (size_t size);

int mem_free (void*);



int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg);

int thread_exit ();

void thread_dispatch ();


int sem_open (sem_t* handle, unsigned init);

int sem_close (sem_t handle);

int sem_wait (sem_t id);

int sem_signal (sem_t id);

int sem_trywait(sem_t id);




int sem_timedwait(sem_t id, time_t timeout);

int time_sleep (time_t);

char getc ();

void putc (char);


#endif
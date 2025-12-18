#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.h"

void Thread::threadWrapper(void* t){
    Thread* thread = (Thread*)t;
    thread->run();
}

Thread:: Thread(void (*body)(void*), void* arg): body(body), arg(arg) {}

Thread::Thread () : body(nullptr), arg(nullptr) {}

void Thread::dispatch (){
    thread_dispatch();
}

Thread::~Thread (){
    delete &myHandle;
}

int Thread::sleep(time_t time) {
   return time_sleep(time);
}

int Thread::start (){
    if(body) // nit treba da izvrsava ono sto je zadato u body-u
    {
        return thread_create(&myHandle, body, arg);
    }else{ // nit treba da izvrsava ono sto je u okviru run metode

        return thread_create(&myHandle, &threadWrapper, this);
    }
}

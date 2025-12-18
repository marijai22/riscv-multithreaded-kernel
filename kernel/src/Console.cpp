#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.h"

char Console::getc (){
    char k = ::getc();
    return k;
}
void Console::putc (char c){
    ::putc(c);
}
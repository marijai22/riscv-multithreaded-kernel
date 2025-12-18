#ifndef _RISCV_HPP
#define _RISCV_HPP

#include "../lib/hw.h"

class Riscv {
public:
    //supervisor trap
    static void supervisorTrap();

    static void firstTimeChosen();

    //citanje registra a0
    static uint64 r_a0();
    static uint64 r_a1();
    static uint64 r_a2();
    static uint64 r_a3();
    static uint64 r_a4();
    static uint64 r_a6();
    static uint64 r_a7();

   // static void w_a0(uint64 value);

    static uint64 r_scause();

    //write register scause
    static void w_scause(uint64 scause);

    //read register sepc
    static uint64 r_sepc();

    //write register sepc
    static void w_sepc(uint64 sepc);

    //read register stvec
    static uint64 r_stvec();

    //write register stvec
    static void w_stvec(uint64 stvec);

    //read register stval
    static uint64 r_stval();

    //write register stval
    static void w_stval(uint64 stval);

    enum BitMaskSip
    {
        SIP_SSIP = (1 << 1), //software interrupt pending
        SIP_STIP = (1 << 5), //trap interrupt pending
        SIP_SEIP = (1 << 9), //external interrupt pending
    };

    // mask set register sip
    static void ms_sip(uint64 mask);

    // mask clear register sip
    static void mc_sip(uint64 mask);

    //read register sip
    static uint64 r_sip();

    // write register sip
    static void w_sip(uint64 sip);

    enum BitMaskSstatus
    {
        SSTATUS_SIE = (1 << 1),
        SSTATUS_SPIE = (1 << 5),
        SSTATUS_SPP = (1 << 8),
    };

    // mask set register sstatus
    static void ms_sstatus(uint64 mask);

    // mask clear register sstatus
    static void mc_sstatus(uint64 mask);

    //read register sstatus
    static uint64 r_sstatus();

    // write register sstatus
    static void w_sstatus(uint64 sstatus);

private:
    static void handleSupervisorTrap();
};

inline uint64 Riscv::r_a0(){ // za citanje koda instrukcije
    uint64 volatile a0;
    __asm__ volatile("mv %[savedValue], a0" :  [savedValue] "=r" (a0));
    return a0;
}

inline uint64 Riscv::r_a1(){ // za citanje prvog argumenta
    uint64 volatile a1;
    __asm__ volatile("mv %[savedValue], a1" :  [savedValue] "=r" (a1));
    return a1;
}

inline uint64 Riscv::r_a2(){ // za citanje koda instrukcije
    uint64 volatile a2;
    __asm__ volatile("mv %[savedValue], a2" :  [savedValue] "=r" (a2));
    return a2;
}

inline uint64 Riscv::r_a3(){ // za citanje koda instrukcije
    uint64 volatile a3;
    __asm__ volatile("mv %[savedValue], a3" :  [savedValue] "=r" (a3));
    return a3;
}

inline uint64 Riscv::r_a4(){ // za citanje koda instrukcije
    uint64 volatile a4;
    __asm__ volatile("mv %[savedValue], a4" :  [savedValue] "=r" (a4));
    return a4;
}

inline uint64 Riscv::r_a6(){ // za citanje koda instrukcije
    uint64 volatile a6;
    __asm__ volatile("mv %[savedValue], a6" :  [savedValue] "=r" (a6));
    return a6;
}

inline uint64 Riscv::r_a7(){ // za citanje koda instrukcije
    uint64 volatile a7;
    __asm__ volatile("mv %[savedValue], a7" :  [savedValue] "=r" (a7));
    return a7;
}

inline uint64 Riscv::r_scause()

{
    uint64 volatile scause;
    __asm__ volatile("csrr %[scause], scause" : [scause] "=r" (scause));
    return scause;
}

inline void Riscv::w_scause(uint64 scause)
{
    __asm__ volatile("csrw scause, %[scause]" : : [scause] "r" (scause));
}

inline uint64 Riscv::r_sepc()
{
    uint64 volatile sepc;
    __asm__ volatile("csrr %[sepc], sepc": [sepc] "=r" (sepc));
    return sepc;

}

inline void Riscv::w_sepc(uint64 sepc)
{
    __asm__ volatile("csrw sepc, %[sepc]" : : [sepc] "r" (sepc));
}

inline uint64 Riscv::r_stvec()
{
    uint64 volatile stvec;
    __asm__ volatile("csrr %[stvec], stvec": [stvec] "=r" (stvec));
    return stvec;

}

inline void Riscv::w_stvec(uint64 stvec)
{
    __asm__ volatile("csrw stvec, %[stvec]" : : [stvec] "r" (stvec));
}

inline uint64 Riscv::r_stval()
{
    uint64 volatile stval;
    __asm__ volatile("csrr %[stval], stval": [stval] "=r" (stval));
    return stval;

}

inline void Riscv::w_stval(uint64 stval)
{
    __asm__ volatile("csrw stval, %[stval]" : : [stval] "r" (stval));
}

inline void Riscv::ms_sip(uint64 mask)
{
    __asm__ volatile("csrs sip, %[mask]" : : [mask] "r" (mask));
}

inline void Riscv::mc_sip(uint64 mask)
{
    __asm__ volatile("csrc sip, %[mask]" : : [mask] "r" (mask));
}

inline uint64 Riscv::r_sip()
{
    uint64 volatile sip;
    __asm__ volatile("csrr %[sip], sip": [sip] "=r" (sip));
    return sip;

}

inline void Riscv::w_sip(uint64 sip)
{
    __asm__ volatile("csrw sip, %[sip]" : : [sip] "r" (sip));
}

inline void Riscv::ms_sstatus(uint64 mask)
{
    __asm__ volatile ("csrs sstatus, %[mask]" : : [mask] "r"(mask));
}

inline void Riscv::mc_sstatus(uint64 mask)
{
    __asm__ volatile("csrc sstatus, %[mask]" : : [mask] "r" (mask));
}

inline uint64 Riscv::r_sstatus()
{
    uint64 volatile sstatus;
    __asm__ volatile("csrr %[sstatus], sstatus": [sstatus] "=r" (sstatus));
    return sstatus;

}

inline void Riscv::w_sstatus(uint64 sstatus)
{
    __asm__ volatile("csrw sstatus, %[sstatus]" : : [sstatus] "r" (sstatus));
}
#endif
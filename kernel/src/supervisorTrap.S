.extern _ZN5Riscv20handleSupervisorTrapEv
.global _ZN5Riscv14supervisorTrapEv
.type _ZN5Riscv14supervisorTrapEv, @function
.align 4

_ZN5Riscv14supervisorTrapEv:

// cuvamo kontekst
    addi sp, sp, -256; //alociramo prostor za 32 registra
    .irp index 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    sd x\index, \index * 8(sp)     //po 8B za smestanje vrednosti registara
    .endr

// pozivamo f-ju za obradu svih prekida
    call _ZN5Riscv20handleSupervisorTrapEv

// restauracija konteksta
    .irp index 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256;

    sret
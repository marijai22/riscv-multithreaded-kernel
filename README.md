# 🖥️ Operating System Kernel – C / C++ / RISC-V Assembly

This project is a handcrafted operating system kernel created for the *Operating Systems* course. It has been built entirely from the ground up using C, C++, and RISC-V Assembly, focusing on core low-level OS principles and providing a clear view into how real operating systems operate beneath the surface.

## 🧠 Project Summary

The kernel aims to deliver a foundational yet practical operating system experience by supporting:

- Thread creation, termination, and switching
- Cooperative multitasking using a custom scheduler
- Synchronization through semaphores
- Handling of hardware and software interrupts
- Console input/output operations
- System call mechanisms with user-mode and kernel-mode separation

All functionality is implemented from scratch without any dependencies on external libraries or existing operating system services.

## 🛠️ Technologies

- **C/C++** – for implementing kernel logic, control flow, and data structures  
- **RISC-V Assembly** – for context switching, interrupt handling, and system-level routines  
- **RISCV Emulator / QEMU** – used for testing and simulating hardware behavior  

## 🔧 Key Features

- Fully custom cooperative thread scheduler  
- Support for thread lifecycle: creation, starting, dispatching, and exiting  
- Semaphore-based synchronization and mutual exclusion  
- Comprehensive interrupt handling with timer-driven context switching  
- Clean separation between user and kernel modes using `ecall` system calls  
- Console communication with internal buffering for input/output  
- Kernel and user thread support with distinct execution modes  
- Integrated memory management via custom `new` / `delete` operators  

---

This kernel project serves as a practical exercise in understanding operating system internals and building reliable low-level software from scratch.

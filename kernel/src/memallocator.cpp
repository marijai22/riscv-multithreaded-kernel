#include "../h/memallocator.hpp"
#include "../h/syscall_c.h"
#define DEALLOCATOR_ERROR -1


bool MemoryAllocator::isValidPointer(uint64 *p) {
    if (p == nullptr) {
        return false;
    }

    FreeBlock* ptr = (FreeBlock*)((uint8*)p - sizeof(FreeBlock));

    if (ptr->size <= 0) {
        return false;
    }

    // proveri da li je zaglavlje unutar granica memorijskog prostora
    if ((uint8*)ptr < (uint8*)HEAP_START_ADDR || (uint8*)ptr >= (uint8*)HEAP_END_ADDR) {
        return false; // Zaglavlje nije unutar granica memorijskog prostora
    }

    // ptoveri da li je header validan FreeBlock
    if (ptr->next == nullptr && ptr->size > 0) {
        return true;
    }
    return false;
}

MemoryAllocator& MemoryAllocator::getInstance() {
    static MemoryAllocator instance;
    return instance;
}

void* MemoryAllocator::allocate(size_t size){  //avgust 2021. uzet kao inspiracija
    if(size <= 0) return nullptr;


    FreeBlock *curr = freeBlockHead, *prev = nullptr;

    // pokusavamo da nadjem po frist fit, prvi slobodan odgvoracajuci blok
    for(; curr; prev = curr, curr = curr->next){
        if(curr->size >= size) break;
    }

    // ako nije pronadjen odgovarajuci blok
    if(curr == nullptr) return nullptr;

    // gledamo da li mozemo da preostali slobodan deo gledamo kao novi FreeBlock ili da ga pridruzimo alokaciji
    size_t remainingSize =  curr->size - size;
    if(remainingSize >= sizeof(FreeBlock) + MEM_BLOCK_SIZE){
        curr->size = size;
        size_t offset = size + sizeof(FreeBlock);
        FreeBlock* newBlk = (FreeBlock*)((char*)curr + offset);
        if(prev){
            prev->next = newBlk;
        }else{
            freeBlockHead = newBlk;
        }
        newBlk->next = curr->next;
        newBlk->size = remainingSize - sizeof(FreeBlock);
    }else{
        if(prev){
            prev->next = curr->next;
        }else{
            freeBlockHead = curr->next;
        }
    }
    curr->next = nullptr;
    return (char*)curr+sizeof(FreeBlock);


}

int MemoryAllocator::deallocate(void* p){ //septembar 2015. uzet kao inspiracija
    if(p == nullptr) return DEALLOCATOR_ERROR;
    //if(((char*)HEAP_END_ADDR-(char*)p)<0 ||(char*)p < (char*)HEAP_START_ADDR + sizeof(FreeBlock)){return DEALLOCATOR_ERROR;}
    FreeBlock* newBlock = (FreeBlock*)((char*)p - sizeof(FreeBlock));

    FreeBlock* prevBlock = nullptr;
    FreeBlock* currBlock = freeBlockHead;

    // trazimo slobodan blok koji je isao pre nas
    while (currBlock && (char*)currBlock < (char*)newBlock) {
        prevBlock = currBlock;
        currBlock = currBlock->next;
    }

    // umetanje novog bloka u listu
    newBlock->next = currBlock;

    // ako umetamo ispred prvog bloka
    if (!prevBlock) {
        freeBlockHead = newBlock;
    } else {
        prevBlock->next = newBlock;
    }

    // pkušaj da spojimo sa sledećim blokom
    if (currBlock && (char*)newBlock + sizeof(FreeBlock) + newBlock->size== (char*)currBlock) {
        // spoji se sa sledećim blokom
        newBlock->size += currBlock->size;
        newBlock->next = currBlock->next;
    }

    // pokušaj da spojimo sa prethodnim blokom
    if (prevBlock && (char*)prevBlock + sizeof(FreeBlock) + prevBlock->size == (char*)newBlock) {
        // spoji se sa prethodnim blokom
        prevBlock->size += newBlock->size;
        prevBlock->next = newBlock->next;
    }

    return 0;
}
#include "proc.h"
#include "mm.h"

MCS_LOCK* p_mm_lock = NULL;

PUBLIC void initKernelDynamicMem() {
    MEM_BLOCK* pblock   =  (MEM_BLOCK*)KERNEL_DYNAMIC_MEM_BASE;
    pblock->len         = sizeof(MEM_BLOCK);
    pblock->next        = pblock + 1;
    pblock->next->len   = KERNEL_DYNAMIC_MEM_LEN - pblock->len;
    pblock->next->next  = NULL;
    disp_T64(pblock->next);
}

PUBLIC void* kmalloc(int size) {
    MEM_BLOCK* pblock =  (MEM_BLOCK*)KERNEL_DYNAMIC_MEM_BASE;
    void* p = NULL;
    int _size = size + sizeof(int);
    
    MCS_LOCK me;
    lock_mcs(&p_mm_lock, &me);

    while (pblock->next != NULL) {
        if (pblock->next->len >= _size) {
            int* plen = (int *)(pblock->next);
            if (pblock->next->len <= (_size / sizeof(void*) + 1) * sizeof(void*)) {   // 按照指针长度对齐分配内存，减小内存碎片数量
                pblock->next = pblock->next->next;
            } else {
                if (_size % sizeof(void *) > 0) {
                    _size = (_size / sizeof(void*) + 1) * sizeof(void*);
                }
                MEM_BLOCK* pnext = (MEM_BLOCK *)((void *)(pblock->next) + _size);
                pnext->next = pblock->next->next;
                pnext->len = pblock->next->len - _size;
                pblock->next = pnext;
            }
            *plen = _size;
            plen++;
            p = (void*) plen;
            break;
        }
        pblock = pblock->next;
    }
    
    unlock_mcs(&p_mm_lock, &me);
    return p;
}

PUBLIC void kfree(void* p) {
    void* pstart = p;
    pstart -= sizeof(int);
    void* pend = pstart + *((int*)pstart);
    
    MEM_BLOCK* pblock =  (MEM_BLOCK*)KERNEL_DYNAMIC_MEM_BASE;
    MEM_BLOCK* pb = (MEM_BLOCK*)pstart;
    
    MCS_LOCK me;
    lock_mcs(&p_mm_lock, &me);
    
    while (pblock->next != NULL) {
         if ((unsigned long)pend < (unsigned long)(pblock->next)) {
             pb->next = pblock->next;
             pblock->next = pb;
         } else if ((unsigned long)pend == (unsigned long)(pblock->next)) {
             pb->next = pblock->next->next;
             pb->len += pblock->next->len;
             pblock->next = pb;
         } else {
             void* pblockend = (void *)(pblock->next) + pblock->next->len;
             
             if (pstart < pblockend) {
                 // TODO: show error
             } else if (pstart == pblockend) {
                 pblock->next->len += pb->len;
             } else {
                 if (pblock->next->next != NULL) {
                     pblock = pblock->next;
                 } else {
                     pb->next = NULL;
                     pblock->next->next = pb;
                 }
             }
         }
    }
    
    unlock_mcs(&p_mm_lock, &me);
}


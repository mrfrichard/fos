#ifndef _FOS_MM_H_
#define _FOS_MM_H_

#include "type.h"
#include "const.h"

#define DIRECT_MAPPING_PHYS_BASE (0xffff880000010000)

#define _KERNEL_DYNAMIC_MEM_BASE (0x30000)

#define KERNEL_DYNAMIC_MEM_BASE (DIRECT_MAPPING_PHYS_BASE + _KERNEL_DYNAMIC_MEM_BASE)
#define KERNEL_DYNAMIC_MEM_LEN (0x60000)

#define REV_TAG (0xffffffffffffffff)

typedef struct _mem_block {
    T64 len;
    struct _mem_block* next;
} MEM_BLOCK;

PUBLIC static inline void initKernelDynamicMem() {
    MEM_BLOCK* pblock   =  (MEM_BLOCK*)KERNEL_DYNAMIC_MEM_BASE;
    pblock->len         = sizeof(MEM_BLOCK);
    pblock->next        = pblock + 1;
    pblock->next->len   = KERNEL_DYNAMIC_MEM_LEN - pblock->len;
    pblock->next->next  = NULL;
}

PUBLIC void* kmalloc(int size);
PUBLIC void* kmallocAlign16(int size);
PUBLIC void kfree(void* p);

#endif /* _FOS_I8259_H_ */

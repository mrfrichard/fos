#ifndef _FOS_MM_H_
#define _FOS_MM_H_

#include "const.h"
#include "proto.h"
#include "string.h"

#define KERNEL_BASE (0xffff880000000000)
#define DIRECT_MAPPING_PHYS_BASE (KERNEL_BASE + 0x10000)

#define _KERNEL_DYNAMIC_MEM_BASE (0x30000)

#define KERNEL_DYNAMIC_MEM_BASE (DIRECT_MAPPING_PHYS_BASE + _KERNEL_DYNAMIC_MEM_BASE)
#define KERNEL_DYNAMIC_MEM_LEN (0x60000)

#define REV_TAG (0xffffffffffffffff)

#define MemChkBuf (0x901D0)
#define dwMCRNumber (0x902D0)

#define MEM_CHECK_BUFFER (KERNEL_BASE + MemChkBuf)
#define MEM_MCR_NUMBER_ADDRESS (KERNEL_BASE + dwMCRNumber)

#define mcr_type_AddressRangeMemory (1)
#define mcr_type_AddressRangeReserved (2)

#define USEABLE_MEM_RANGE_ARRAY_SIZE (32)

#define KERNEL_RANGE_ADDRESS (0x10000)
#define KERNEL_RANGE_LENGTH (0x30000)

#define PG_P    (1)
#define PG_RWW  (2)
#define PG_USS  (0)
#define PG_USU  (4)

typedef struct _mem_block {
    T64 len;
    struct _mem_block* next;
} MEM_BLOCK;

typedef struct _mem_range {
    T64 address;
    T64 length;
}MEM_RANGE;

typedef struct _mcr {
    MEM_RANGE range;
    T32 type;
} __attribute__ ((packed)) MCR;

extern T64 totalMemSize;
extern MEM_RANGE* p_mem_ranges;


inline void showMcrInfo (unsigned char i, MCR* pmcr){
    disp_int(i);
    disp_str(". base = ");
    disp_T64(pmcr->range.address);
    disp_str(" lenth = ");
    disp_T64(pmcr->range.length);
    disp_str(" type = ");
    switch (pmcr->type) {
        case mcr_type_AddressRangeMemory:
            disp_str("mem");
            break;
        case mcr_type_AddressRangeReserved:
            disp_str("rev");
            break;
        default:
            disp_str("undef");
            break;
    }
    disp_str("\n");
};


inline T64 allocPagePage(T64 current, MEM_RANGE* range) {
    MEM_RANGE *p = range;
    while ((p->length > 0) && (p->address + p->length) < current) {
        p++;
    }

    if (p->length == 0) {
        disp_str("alloc pagepage failed, for current is to large, current = ");
        disp_T64(current);
        disp_str("\n");
        return NULL;
    }

    T64 start = current;

    while ((start != NULL) && (p->address + p->length) < (start + 0x1000)) {
        p++;
        start = p->address;
    }

    if (start == NULL) {
        disp_str("alloc pagepage failed, for not enough range, current = ");
        disp_T64(current);
        disp_str("\n");
    }

    return start;
}

PUBLIC static inline void initKernelMem() {
    // get mem information form loader data (int 15h, which is called by loader)
    totalMemSize = 0;
    unsigned char useableMemRangeNumber = 0;
    MCR* pmcr = (MCR*)MEM_CHECK_BUFFER;
    MEM_RANGE useableMemRanges[USEABLE_MEM_RANGE_ARRAY_SIZE];

    memset(useableMemRanges, NULL, sizeof(useableMemRanges));

    if (pmcr->type == mcr_type_AddressRangeMemory) {
        useableMemRanges[useableMemRangeNumber].address = pmcr->range.address;
        useableMemRanges[useableMemRangeNumber].length = KERNEL_RANGE_ADDRESS - useableMemRanges[useableMemRangeNumber].address;
        useableMemRangeNumber++;
        useableMemRanges[useableMemRangeNumber].address = KERNEL_RANGE_ADDRESS + KERNEL_RANGE_LENGTH;
        useableMemRanges[useableMemRangeNumber].length = pmcr->range.length - useableMemRanges[useableMemRangeNumber].address;
    }
    showMcrInfo(0, pmcr);
    useableMemRangeNumber++;
    pmcr++;

    MEM_RANGE* prange = useableMemRanges + 2;
    for (unsigned char i = 1; i < *((unsigned char*)MEM_MCR_NUMBER_ADDRESS); i++) {
        if (pmcr->type == mcr_type_AddressRangeMemory) {
            useableMemRangeNumber++;
            memcpy(prange, &(pmcr->range), sizeof(MEM_RANGE));
            prange++;
        }
        showMcrInfo(i, pmcr);
        pmcr++;
    }

    pmcr--;
    totalMemSize = pmcr->range.address + pmcr->range.length;
    T64 pteItemNum = totalMemSize / 0x1000;
//    T64 pteNum = pteItemNum / 0x200;
//    T64 pdeNum = pteNum / 0x200;
//    T64 pdpteNum = pdeNum / 0x200;

//    disp_str("Total Mem size = ");
//    disp_T64(totalMemSize);
//    disp_str("\n");
//    disp_str("pteItemNum  = ");
//    disp_T64(pteItemNum);
//    disp_str("\n");
//    disp_str("pteNum  = ");
//    disp_T64(pteNum);
//    disp_str("\n");
//    disp_str("pdeNum  = ");
//    disp_T64(pdeNum);
//    disp_str("\n");
//    disp_str("pdpteNum  = ");
//    disp_T64(pdpteNum);
//    disp_str("\n");

    T64* pplm4e = (void*)0xffffffffffffffff;

    __asm__ __volatile__ (
        "movq %%cr3, %0"
        : "=r"(pplm4e)
    );

    pplm4e = (T64*)((T64)pplm4e & 0xfffffffffffff000);
    T64* ppdpte = (T64*)(*pplm4e & 0xfffffffffffff000);
    T64* ppde = (T64*)(*ppdpte & 0xfffffffffffff000);
    T64* current= (T64*)((*ppde & 0xfffffffffffff000) + 0x1000);
    T64  pteItemCounter = 0x200;

    while (pteItemCounter < pteItemNum) {
        if (!(pteItemCounter % 0x8000000)) {
            T64 address = allocPagePage((T64)current, useableMemRanges);
            if (address != NULL) {
                T64 index = pteItemCounter / 0x8000000;
                index %= 0x200;
                *(pplm4e + index) = address | PG_P | PG_USS | PG_RWW;
                ppdpte = (T64*)address;
                current = (T64*)(address + 0x1000);
            } else {
                break;
            }
        }

        if (!(pteItemCounter % 0x40000)) {
            T64 address = allocPagePage((T64)current, useableMemRanges);
            if (address != NULL) {
                T64 index = pteItemCounter / 0x40000;
                index %= 0x200;
                *(ppdpte + index) = address | PG_P | PG_USS | PG_RWW;
                ppde = (T64*)address;
                current = (T64*)(address + 0x1000);
            } else {
                break;
            }
        }

        if (!(pteItemCounter % 0x200)) {
            T64 address = allocPagePage((T64)current, useableMemRanges);
            if (address != NULL) {
                T64 index = pteItemCounter / 0x200;
                index %= 0x200;
                *(ppde + index) = address | PG_P | PG_USS | PG_RWW;
//                disp_T64_pos(ppde + index, 0);
//                disp_T64_pos(index, 20);
                current = (T64*)address;
            } else {
                break;
            }
        }

        *(current) = (pteItemCounter * 0x1000) | PG_P | PG_USS | PG_RWW;
        pteItemCounter ++;
        current++;
    }
}

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

#endif /* _FOS_MM_H_ */

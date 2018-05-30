#include "type.h"
//#include "const.h"
#include "protect.h"
#include "global.h"
#include "proto.h"
#include "string.h"


PUBLIC void cstart() {
    disp_str("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n------------\"cstrt\" begins -----------\n");
    memcpy(&gdt, (void*)(*((T64*)(&gdtPtr[2]))), *((T16*)(&gdtPtr[0])));
    T16* p_gdt_limit = (T16*)(&gdtPtr[0]);
    T64* p_gdt_base = (T64*)(&gdtPtr[2]);
    *p_gdt_limit = GDT_SIZE * sizeof(Descriptor);
    *p_gdt_base = (T64) & gdt;


    T16* p_idt_limit = (T16*)(&idtPtr[0]);
    T64* p_idt_base = (T64*)(&idtPtr[2]);
    *p_idt_limit = IDT_SIZE * sizeof(Gate);
    *p_idt_base = (T64) & idt;

    init_prot();
    disp_str("------------\"cstrt\" finished -----------\n");

}

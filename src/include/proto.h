#ifndef _FOS_PROTO_H_
#define _FOS_PROTO_H_

#include "type.h"

PUBLIC void outb(TPort port, T8 value);
PUBLIC T8 inb(TPort port);
PUBLIC int disable_irq(int irq);
PUBLIC void enable_irq(int irq);

PUBLIC void disp_color_str_pos(unsigned short color, char* pszinfo, unsigned pos);
PUBLIC void disp_color_str(unsigned short  color, char* pszinfo);
PUBLIC void disp_str(char* pszinfo);
PUBLIC void disp_str_pos(char* pszinfo, unsigned pos);

PUBLIC char* itoa(char *str, int num);
PUBLIC void disp_T64(T64 v);
PUBLIC char* ltoa(char *str, T64 num);

PUBLIC void disp_int(int v);
PUBLIC void disp_T64(T64 v);
PUBLIC void disp_int_pos(int v, unsigned pos);
PUBLIC void disp_T64_pos(T64 v, unsigned pos);


PUBLIC void hwint00();
PUBLIC void hwint01();
PUBLIC void hwint02();
PUBLIC void hwint03();
PUBLIC void hwint04();
PUBLIC void hwint05();
PUBLIC void hwint06();
PUBLIC void hwint07();
PUBLIC void hwint08();
PUBLIC void hwint09();
PUBLIC void hwint10();
PUBLIC void hwint11();
PUBLIC void hwint12();
PUBLIC void hwint13();
PUBLIC void hwint14();
PUBLIC void hwint15();
PUBLIC void sys_call();

PUBLIC void divide_error();
PUBLIC void single_step_exception();
PUBLIC void nmi();
PUBLIC void breakpoint_exception();
PUBLIC void overflow();
PUBLIC void bounds_check();
PUBLIC void inval_opcode();
PUBLIC void copr_not_available();
PUBLIC void double_fault();
PUBLIC void copr_seg_overrun();
PUBLIC void inval_tss();
PUBLIC void segment_not_present();
PUBLIC void stack_exception();
PUBLIC void general_protection();
PUBLIC void page_fault();
PUBLIC void copr_error();
PUBLIC void math_fault();
PUBLIC void alignment_check();
PUBLIC void machine_check();
PUBLIC void simd_fault();
PUBLIC void virtualization_exception();

PUBLIC void restart();
#endif /* _FOS_PROTO_H_ */

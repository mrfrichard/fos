#include "i8259.h"
#include "proto.h"
#include "global.h"


PUBLIC void spurious_irq(int irq) {
    disp_str("spurious_irq: ");
    disp_int(irq);
    disp_str("\n");
}

PUBLIC void put_irq_handler(int irq, IrqHandler handler) {
    disable_irq(irq);
    irq_table[irq] = handler;
}

PUBLIC void setup_irq(int irq, IrqHandler handler) {
    put_irq_handler(irq, handler);
    enable_irq(irq);
}


PUBLIC void init_8259A() {
    // Master 8259, ICW1:
    outb(INT_M_CTL, 0x11);
    // Slave 8259, ICW1:
    outb(INT_S_CTL, 0x11);
    // Master 8259, ICW2: 设置‘主8259’的中断入口地址为 0x20(INT_VECTOR_IRQ0)
    
    outb(INT_M_CTLMASK, INT_VECTOR_IRQ0);
    // Slave 8259, ICW2: 设置‘从8259’的中断入口地址为 0x28(INT_VECTOR_IRQ8)
    outb(INT_S_CTLMASK, INT_VECTOR_IRQ8);
    // Master 8259, ICW3. IR2 对应‘从8259’
    outb(INT_M_CTLMASK, 0x4);
    // Slave 8259, ICW3. 对应‘主8259’的IR2
    outb(INT_S_CTLMASK, 0x2);
    // Master 8259, ICW4:
    outb(INT_M_CTLMASK, 0x1);
    // Slave 8259, ICW4:
    outb(INT_S_CTLMASK, 0x1);
    // Master 8259, OCW1:
    outb(INT_M_CTLMASK, 0xff);
    // Slave 8259, OCW1:屏蔽‘从8259’所有中断
    outb(INT_S_CTLMASK, 0xff);

    for (int i = 0; i < NR_IRQ; i++) {
        irq_table[i] = spurious_irq;
    }
}


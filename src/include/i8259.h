#ifndef _FOS_I8259_H_
#define _FOS_I8259_H_

#include "type.h"

/* 8259A interrupt controller ports. */
#define INT_M_CTL       (0x20)      // I/O for interrupt controller             <Master>
#define INT_M_CTLMASK   (0x21)      // setting bits in this port disables ints  <Master>
#define INT_S_CTL       (0xA0)      // I/O for second interrupt controller      <Slave>
#define INT_S_CTLMASK   (0xA1)      // setting bits in this port disables ints  <Slave>

#define TIMER0          (0x40)
#define TIMER_MODE      (0x43)
#define RATE_GENERATOR  (0x34)
#define TIME_FREQ       (1193182L)
#define HZ              (100)
#define TIME_CNT        (TIME_FREQ / HZ)

PUBLIC void spurious_irq(int irq);
PUBLIC void put_irq_handler(int irq, IrqHandler handler);
PUBLIC void setup_irq(int irq, IrqHandler handler);
PUBLIC void init_8259A();


#endif /* _FOS_I8259_H_ */

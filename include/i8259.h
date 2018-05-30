#ifndef _FOS_I8259_H_
#define _FOS_I8259_H_

#include "type.h"

PUBLIC void spurious_irq(int irq);
PUBLIC void put_irq_handler(int irq, IrqHandler handler);
PUBLIC void setup_irq(int irq, IrqHandler handler);
PUBLIC void init_8259A();


#endif /* _FOS_I8259_H_ */

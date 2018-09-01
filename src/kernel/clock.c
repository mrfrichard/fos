#include "const.h"
#include "protect.h"
#include "global.h"
#include "proto.h"
#include "i8259.h"
#include "proc.h"
#include "clock.h"

PUBLIC void clock_handler(int irq) {
    ticks++;

    p_proc_ready->ticks--;
    if (k_reenter != 0) {
        return;
    }

    schedule();
}

PUBLIC void init_clock() {
    outb(TIMER_MODE, RATE_GENERATOR);
    outb(TIMER0, (T8)TIME_CNT);
    outb(TIMER0, (T8)(TIME_CNT >> 8));
    setup_irq(CLOCK_IRQ, clock_handler);
}

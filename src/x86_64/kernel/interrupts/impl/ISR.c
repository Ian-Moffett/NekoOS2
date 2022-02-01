#include "../ISR.h"

unsigned long long ticks = 0;

void irq0_isr(int_frame_t*) {
    if (ticks >= 10000) {
        ticks = 0;
    }

    ++ticks;
    outportb(0x20, 0x20);    
}

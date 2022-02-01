#include "../PIT.h"


void pit_set_freq(unsigned int frequency) {
    unsigned int divisor = 1193182 / frequency;
    outportb(0x43, 0x36);             // Command byte.
    outportb(0x40, divisor & 0xFF);   // Divisor low byte.
    outportb(0x40, divisor >> 8);     // Divisor high byte.
    outportb(0x21, 0xFF);
}


void sleep(unsigned int _ticks) {
    extern unsigned long long ticks;

    unsigned long long left = ticks + _ticks;
    while (ticks < left);
}

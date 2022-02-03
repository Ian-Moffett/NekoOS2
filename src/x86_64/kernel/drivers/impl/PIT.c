#include "../PIT.h"

unsigned int ticks = 0;


void pit_set_freq(unsigned int frequency) {
    unsigned int divisor = 1193182 / frequency;
    outportb(0x43, 0x36);             // Command byte.
    outportb(0x40, divisor & 0xFF);   // Divisor low byte.
    outportb(0x40, divisor >> 8);     // Divisor high byte.
    outportb(0x21, 0xFF);
}


void sleep(unsigned int _ticks) {
    unsigned long long left = ticks + _ticks;
    while (ticks < left);
}


__attribute__((interrupt)) void _irq0_isr(int_frame64_t*) {
    __asm__ __volatile__("cli");

    ++ticks;

    if (ticks > 3000) {
        ticks = 0;
    }

    outportb(0x20, 0x20);
    __asm__ __volatile__("sti");
}

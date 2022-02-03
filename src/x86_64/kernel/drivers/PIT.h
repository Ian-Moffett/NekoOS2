#ifndef PIT_H
#define PIT_H

#include "IO.h"
#include "../interrupts/IDT.h"

void pit_set_freq(unsigned int frequency);
void sleep(unsigned int _ticks);
__attribute__((interrupt)) void _irq0_isr(int_frame64_t*);

#endif

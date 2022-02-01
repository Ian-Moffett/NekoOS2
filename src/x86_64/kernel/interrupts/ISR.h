#ifndef ISR_H
#define ISR_H

#include "IDT.h"
#include "../drivers/IO.h"

__attribute__((interrupt)) void irq0_isr(int_frame_t*);


#endif

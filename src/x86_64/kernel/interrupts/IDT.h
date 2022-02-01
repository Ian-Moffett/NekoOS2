#ifndef IDT_H
#define IDT_H

#include "../drivers/IO.h"

#define TRAP_GATE_FLAGS 0x8F
#define INT_GATE_FLAGS 0x8E

typedef struct {
    unsigned short isr_addr_low;
    unsigned short kernel_cs;
    unsigned char reserved;
    unsigned char attr;
    unsigned short isr_addr_high;
} __attribute__((packed)) idt_entry_t;


typedef struct {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed)) idtr_t;


typedef struct {
    unsigned int eip;
    unsigned int cs;
    unsigned int eflags;
    unsigned int sp;
    unsigned int ss;
} __attribute__((packed)) int_frame_t;


void set_idt_entry(unsigned char entry, void* isr, unsigned char flags);
void idt_install();

#endif

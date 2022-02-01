#include "../IDT.h"

static idt_entry_t idt[256];
static idtr_t idtr;


void set_idt_entry(unsigned char entry, void* isr, unsigned char flags) {
    idt[entry].isr_addr_low = (unsigned int)isr & 0xFFFF;
    idt[entry].kernel_cs = 0x08;
    idt[entry].reserved = 0x0;
    idt[entry].attr = flags;
    idt[entry].isr_addr_high = ((unsigned int)isr >> 16) & 0xFFFF;
}


void idt_install() {
    // Remap PIC.
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);

    idtr.limit = (unsigned short)(sizeof(idt));
    idtr.base = (unsigned int)&idt;
    __asm__ __volatile__("lidt %0" : : "memory"(idtr));
}
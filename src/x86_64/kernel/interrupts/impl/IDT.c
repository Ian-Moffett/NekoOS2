#include "../IDT.h"

static idt_entry64_t idt64[256];
static idtr_t idtr;


void set_idt_entry(unsigned char entry, void* isr, unsigned char flags) {
    uint64_t addr = (uint64_t)isr;
    idt64[entry].isr_addr_low = addr & 0xFFFF;
    idt64[entry].isr_addr_middle = (addr & 0xFFFF0000) >> 16;
    idt64[entry].isr_addr_high = (addr & 0xFFFFFFFF00000000) >> 32;
    idt64[entry].dpl = 0;
    idt64[entry].p = 1;
    idt64[entry].attr = flags;
    idt64[entry].kernel_cs = 0x08;
    idt64[entry].reserved = 0x0;
    idt64[entry].reserved2 = 0x0;
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

    idtr.limit = (unsigned short)(sizeof(idt64));
    idtr.base = (uint64_t)&idt64;
    __asm__ __volatile__("lidt %0" : : "memory"(idtr));
}

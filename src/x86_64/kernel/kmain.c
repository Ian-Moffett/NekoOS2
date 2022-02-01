#include "drivers/VGA.h"
#include "drivers/PIT.h"
#include "drivers/IO.h"
#include "interrupts/IDT.h"
#include "interrupts/exceptions.h"
#include "interrupts/ISR.h"

char* vga = (char*)0xB8000;

void panic(const char* const PANIC_MESSAGE) {
    outportb(0x3D4, 0x0A);
    outportb(0x3D5, 0x20);
    clearScreen(&vga, 0x4, 0xFE);
    kputs("***KERNEL PANIC***", &vga, 2);
    kputs(PANIC_MESSAGE, &vga, 1);
    __asm__ __volatile__("cli; hlt");
}


int _start() {
    idt_install();

    // Exceptions.
    set_idt_entry(0x0, div_0_ex, TRAP_GATE_FLAGS);
    set_idt_entry(0x4, overflow_ex, TRAP_GATE_FLAGS);
    set_idt_entry(0x5, bre_ex, TRAP_GATE_FLAGS);
    set_idt_entry(0x6, invld_opcode_ex, TRAP_GATE_FLAGS);
    set_idt_entry(0x7, dev_na_ex, TRAP_GATE_FLAGS);
    set_idt_entry(0x8, double_fault_ex, TRAP_GATE_FLAGS);
    set_idt_entry(0x9, cso_ex, TRAP_GATE_FLAGS);
    set_idt_entry(0xA, invld_tss_ex, TRAP_GATE_FLAGS);
    set_idt_entry(0xB, seg_np_ex, TRAP_GATE_FLAGS);
    set_idt_entry(0xC, ss_fault_ex, TRAP_GATE_FLAGS);
    set_idt_entry(0xD, gp_fault_ex, TRAP_GATE_FLAGS);
    set_idt_entry(0xE, page_fault_ex, TRAP_GATE_FLAGS);
    set_idt_entry(0xF, float_ex, TRAP_GATE_FLAGS);

    // IMR is 8 bits wide. 0xF => 1111, 0xFF => 11111111.
    // outportb(0x20, 0xFF);

    // Set frequency.
    pit_set_freq(5);
    set_idt_entry(0x20, irq0_isr, INT_GATE_FLAGS);
    outportb(0x21, 0xFF);
    outportb(0x21, inportb(0x21) ^ 0x1);    // 11111111 ^ 00000001 => 11111110
    __asm__ __volatile__("sti");

    clearScreen(&vga, 0x1, 0xE);
    kputs("Nyaa!~", &vga, 1);
    kputs("Hello Master!~", &vga, 2);
    return 0; 
}

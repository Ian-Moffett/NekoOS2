#include "drivers/VGA.h"
#include "drivers/PIT.h"
#include "drivers/IO.h"
#include "drivers/mouse.h"
#include "interrupts/IDT.h"
#include "interrupts/exceptions.h"
#include "memory/heap.h"
#include "memory/vmm.h"


char* vga = (char*)0xB8000;

void panic(const char* const PANIC_MESSAGE) {
    outportb(0x3D4, 0x0A);
    outportb(0x3D5, 0x20);
    clearScreen(&vga, 0x4, 0xFE);
    kputs("***KERNEL PANIC***", &vga, 2);
    kputs(PANIC_MESSAGE, &vga, 1);
    __asm__ __volatile__("cli; hlt");
}


void _lm_entry();


__attribute__((interrupt)) void stub(int_frame64_t*) {}

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
    set_idt_entry(0x80, stub, INT_GATE_USER_FLAGS);

    // IMR is 8 bits wide. 0xF => 1111, 0xFF => 11111111.
    // outportb(0x20, 0xFF);

    // Set frequency.
    
    pit_set_freq(5);
    set_idt_entry(0x20, _irq0_isr, INT_GATE_FLAGS);
    outportb(0x21, 0xFF);
    outportb(0x21, inportb(0x21) ^ 0x1);    // 11111111 ^ 00000001 => 11111110
    __asm__ __volatile__("sti");
    clearScreen(&vga, 0x1, 0xE);

    // while (!(inportb(0x60) & 0x1));

    // heap_init((void*)0x10000, 3000); 

    init_vmm();

    kputs("Hello Master!~ <3", &vga, 1);  

    /*
     * TODO: Allow interrupts to work in usermode and use
     * context switching to switch from user to kernel
     * to handle those interrupts and called the ISR's.
     */

    outportb(0x21, 0xFF);
    __asm__ __volatile__("cli");

    return 0; 
}

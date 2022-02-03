#ifndef IDT_H
#define IDT_H

#include "../drivers/IO.h"

#define TRAP_GATE_FLAGS 0x8F
#define INT_GATE_FLAGS 0x8E
#define INT_GATE_USER_FLAGS 0xEE 

#include <stddef.h>
#include <stdint.h>


/*
 * Each entry describes where ISR
 * is located in memory to service
 * the interrupt.
 */

typedef struct {
    uint16_t isr_addr_low;
    uint16_t kernel_cs;         // Code segment where the ISR is.
    uint8_t ist : 3;            // Interrupt stack table. TODO: Research.
    uint8_t reserved : 5;       // Reserved by intel for some reason.
    uint8_t attr : 4;
    uint8_t zero1 : 1;
    uint8_t dpl : 2;            // Descriptor privellege level.
    uint8_t p : 1;              // Present.
    uint16_t isr_addr_middle;   // 64 bits means more bits dummy.
    uint32_t isr_addr_high;
    uint32_t reserved2;         // This was it.
} __attribute__((packed)) idt_entry64_t;


typedef struct {
    unsigned short limit;
    uint64_t base;
} __attribute__((packed)) idtr_t;

typedef struct {
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} __attribute__((packed)) int_frame64_t;


void set_idt_entry(unsigned char entry, void* isr, unsigned char flags);
void idt_install();

#endif

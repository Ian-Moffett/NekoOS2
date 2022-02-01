#include "drivers/VGA.h"
#include "interrupts/IDT.h"


char* vga = (char*)0xB8000;

int _start() {
    idt_install();
    clearScreen(&vga, 0x1, 0xE);
    kputs("Nyaa!~", &vga, 1);
    kputs("Hello Master!~", &vga, 2);
    return 0; 
}

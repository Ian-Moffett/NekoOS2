#include "drivers/VGA.h"

char* vga = (char*)0xB8000;

int _start() {
    clearScreen(&vga, 0x1, 0xE);
    kputs("Nyaa!~", &vga, 1);
    kputs("Hello Master!~", &vga, 2);
    return 0; 
}

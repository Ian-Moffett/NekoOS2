#include "../VGA.h"

void clearScreen(char** vga, unsigned char bg, unsigned char fg) {
    unsigned short attrib = (bg << 4) | (fg & 0x0F);   

    *vga = (char*)0xB8000;

    for (int i = 0; i < 10000; ++i) {
        **vga = ' ';
        *vga += 1;
        **vga = attrib;
        *vga += 1;
    }

    *vga = (char*)0xB8000;
}



void kputs(const char* str, char** vga, unsigned char nlDepth) {
    for (int i = 0; i < strlen(str); ++i) {
        **vga = str[i];
        *vga += 2;
    }

    for (int i = 0; i < nlDepth; ++i) {
        for (int j = 0; j < 80 - strlen(str); ++j) {
            **vga = ' ';
            *vga += 2;
        }
    }
}

#include "../mouse.h"


void mouse_wait(unsigned char a_type) {
    extern char* vga;
	unsigned int timeout = 100000;
	if (!a_type) {
		while (--timeout) {
			if ((inportb(MOUSE_STATUS) & MOUSE_BBIT) == 1) {
				return;
			}
		}
		kputs("Mouse timeout.", &vga, 2);
		return;
	} else {
		while (--timeout) {
			if (!((inportb(MOUSE_STATUS) & MOUSE_ABIT))) {
				return;
			}
		}
		kputs("Mouse timeout.", &vga, 2);
		return;
	}
}


void mouse_write(unsigned char a_write) {
  mouse_wait(1);
  outportb(0x64, 0xD4);
  mouse_wait(1);
  outportb(0x60, a_write);
}

unsigned char mouse_read() {
    mouse_wait(0);
    return inportb(0x60);
}


void mouse_install() {
  unsigned char _status;

  mouse_wait(1);
  outportb(0x64, 0xA8);
 
  // Enable the interrupts
  mouse_wait(1);
  outportb(0x64, 0x20);
  mouse_wait(0);
  _status=(inportb(0x60) | 2);
  mouse_wait(1);
  outportb(0x64, 0x60);
  mouse_wait(1);
  outportb(0x60, _status);
 
  // Tell the mouse to use default settings
  mouse_write(0xF6);
  mouse_read();  //Acknowledge
 
  // Enable the mouse
  mouse_write(0xF4);
  mouse_read();  // Acknowledge
    
   // 32
}

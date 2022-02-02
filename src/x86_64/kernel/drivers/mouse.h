#ifndef MOUSE_H
#define MOUSE_H

#include "IO.h"
#include "VGA.h"

#define PACKETS_IN_PIPE 1024
#define DISCARD_POINT 32

#define MOUSE_IRQ 12

#define MOUSE_PORT   0x60
#define MOUSE_STATUS 0x64
#define MOUSE_ABIT   0x02
#define MOUSE_BBIT   0x01
#define MOUSE_WRITE  0xD4
#define MOUSE_F_BIT  0x20
#define MOUSE_V_BIT  0x08


void mouse_wait(unsigned char a_type);
void mouse_install();
unsigned char mouse_read();
void mouse_write(unsigned char a_write);

#endif

#ifndef INITRD_H
#define INITRD_H

#include "FS.h"
#include "../memory/heap.h"
#include "../util/strings.h"

typedef struct {
    unsigned int nfiles;     // # of files in ramdisk.
} initrd_header_t;


typedef struct {
    unsigned char magic;
    char name[64];          // Filename.
    unsigned int offset;
    unsigned int length;    // Length of file.
} initrd_file_header_t;


struct _FS_NODE* init_initrd(unsigned int loc);


#endif

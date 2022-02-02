#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>


typedef struct _MEM_BLOCK {
    unsigned long long size;
    unsigned char flags;        // 000000000.
    struct _MEM_BLOCK* next;
} mem_block_t;


void heap_init(void* start, unsigned long limit);
void* kmalloc(unsigned long size);
void kfree(void* addr);
void kmem_empty();

#endif

#include "../heap.h"

#define FLAG_FREE 0x1  // 00000001
#define DATA_START(MEM_BLOCK) ((void*)MEM_BLOCK + sizeof(mem_block_t))


static mem_block_t* head;
static mem_block_t* tail;
static unsigned long allocated = 0;
static unsigned int _limit = 0;

void heap_init(void* start, unsigned long limit) {
    _limit = limit;
    head = (mem_block_t*)start;
    head->size = 0;
    head->flags = 0x0;
    head->next = NULL;
    tail = head;
}


static mem_block_t* _first_fit(unsigned long size) {
    mem_block_t* curBlock = head;

    while (curBlock != NULL) {
        if (curBlock->flags & FLAG_FREE && curBlock->size >= size) {
            return curBlock;
        }

        curBlock = curBlock->next;
    }

    return NULL;        // Hole not opened yet.
}


void* kmalloc(unsigned long size) {
    if (allocated + size >= _limit) {
        return NULL;
    }

    mem_block_t* region = _first_fit(size);

    if (region == NULL) {
        mem_block_t* next = (mem_block_t*)DATA_START(tail) + tail->size;
        tail->next = next;
        region = tail->next;
        region->flags = 0x0;
        region->size = size;
        region->next = NULL;
        tail = region;
        allocated += size;
    }

    return DATA_START(region);
}



void kfree(void* addr) {
    mem_block_t* cur = head;

    while (cur != NULL && addr != DATA_START(cur)) {
        cur = cur->next;
    }

    cur->flags |= FLAG_FREE;
}


void kmem_empty() {
    mem_block_t* cur = head;

    while (cur != NULL) {
        cur->flags |= FLAG_FREE;
        cur = cur->next;
    }
}

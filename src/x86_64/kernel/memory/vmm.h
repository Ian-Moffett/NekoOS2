#ifndef VMM_H
#define VMM_H

#define PTE_PAGE_PRESENT          1       /* 00000000000000000000000000000001 */
#define PTE_PAGE_WRITE            2       /* 00000000000000000000000000000010 */
#define PTE_PAGE_USER             4       /* 00000000000000000000000000000100 */
#define PTE_PAGE_WRITETHROUGH     8       /* 00000000000000000000000000001000 */
#define PTE_PAGE_NOT_CACHEABLE 0x10       /* 00000000000000000000000000010000 */
#define PTE_PAGE_ACCESSED      0x20       /* 00000000000000000000000000100000 */
#define PTE_PAGE_DIRTY         0x40       /* 00000000000000000000000001000000 */
#define PTE_PAGE_RESERVED      0x180      /* 00000000000000000000000110000000 */
#define PTE_PAGE_AVAIL         0xE00      /* 00000000000000000000111000000000 */
#define PTE_PAGE_FRAME         0xFFFFF000 /* 11111111111111111111000000000000 */


#define PDE_PAGE_PRESENT          1       /* 00000000000000000000000000000001 */
#define PDE_PAGE_WRITE            2       /* 00000000000000000000000000000010 */
#define PDE_PAGE_USER             4       /* 00000000000000000000000000000100 */
#define PDE_PAGE_WRITETHROUGH     8       /* 00000000000000000000000000001000 */
#define PDE_PAGE_NOT_CACHEABLE 0x10       /* 00000000000000000000000000010000 */
#define PDE_PAGE_ACCESSED      0x20       /* 00000000000000000000000000100000 */
#define PDE_PAGE_DIRTY         0x40       /* 00000000000000000000000001000000 */
#define PDE_PAGE_4MB           0x180      /* 00000000000000000000000110000000 */
#define PDE_PAGE_AVAIL         0xE00      /* 00000000000000000000111000000000 */
#define PDE_PAGE_FRAME         0xFFFFF000 /* 11111111111111111111000000000000 */

#include <stddef.h>
#include <stdint.h>


#define IDENT_OFFSET 0x1L << 46
#define PAGE_LARGE 0x1UL << 7

__attribute__((aligned(4096))) uint64_t ident_pge[512];


void write_cr3(uint64_t* val);
uint64_t* read_cr3(); 


void init_ident_page() {
    for (int i = 0; i < 512; ++i) { ident_pge[i] = 0x0; }
    ident_pge[0] = PDE_PAGE_PRESENT | PDE_PAGE_WRITE | PAGE_LARGE;
}


void map_ident(uint64_t* _l4) {
    // Maps indetity page into random address space.
    // address space: isolated memory enviorement.
    
    size_t offset = IDENT_OFFSET >> 39;
    _l4[offset] = (uint64_t)ident_pge;
    _l4[offset] |= PDE_PAGE_WRITE | PDE_PAGE_PRESENT;
}

void init_vmm() {
    // Init the identity page. 
    uint64_t cr3 = 0;
    __asm__ __volatile__("mov %%cr3, %0" : "=r"(cr3));
    init_ident_page();
    map_ident((uint64_t*)cr3);
}

#endif

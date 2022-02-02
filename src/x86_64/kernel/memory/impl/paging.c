#include "../paging.h"

__attribute__((aligned(4096))) static unsigned int kpge_dir[1024] = {0x00000002};
__attribute__((aligned(4096))) static unsigned int kdummy_pge[1024];

void _init_paging(void*);

void init_paging() {
    for (int i = 0; i < 1024; ++i) {
        kdummy_pge[i] = (i * 0x1000) | 0x3;
    }

    kpge_dir[0] = (unsigned int)&kdummy_pge | 0x3;
    _init_paging((void*)&kpge_dir);
}

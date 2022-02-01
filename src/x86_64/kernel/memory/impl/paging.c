__attribute__((aligned(4096))) unsigned int kpge_dir[1024];
__attribute__((aligned(4096))) static unsigned int kpge_tbl[1024];

int init_paging() {
    for (int i = 0; i < 1024; ++i) {
        kpge_dir[i] = 0x00000002;
        kpge_tbl[i] = (i * 0x1000) | 0x3;
   }

   kpge_dir[0] = (unsigned int)&kpge_tbl | 0x3;
}

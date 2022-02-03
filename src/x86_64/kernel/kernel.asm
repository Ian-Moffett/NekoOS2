bits 32
global _kentry
extern _start
extern _lm_entry

_kentry:
    call _lm_entry

postExec:
    hlt
    jmp postExec

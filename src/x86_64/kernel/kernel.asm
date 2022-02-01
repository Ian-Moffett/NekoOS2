global _kentry
extern _start

_kentry:
    call _start

postExec:
    hlt
    jmp postExec

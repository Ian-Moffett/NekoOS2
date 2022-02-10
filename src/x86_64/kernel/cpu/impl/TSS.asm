bits 64
global _load_tss

_load_tss:
    mov ax, 40
    push bx
    str bx      ; Stores old TSS into dummy location and overwrite.
    pop bx
    ltr ax
    ret

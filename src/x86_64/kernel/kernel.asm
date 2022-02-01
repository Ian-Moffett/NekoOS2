bits 32
global _kentry
extern _start

_kentry:
    ; EDI => dest.
    mov edi, _l4_page_tbl
    push edi
    xor eax, eax
    mov ecx, 1024
    rep stosd       ; (*EDI = EAX++) REP = repeat

    mov edi, _l3_page_tbl
    mov ecx, 1024
    rep stosd

    pop edi
    mov cr3, edi                ; "Hey, CPU this is where the page table will be."
    mov eax, _l3_page_tbl
    add eax, 3
    mov [edi], eax

    xor eax, eax
    mov eax, 3 | (1 << 7) | (1 << 2)       ; Setting entry in l3 page table.
    mov [_l3_page_tbl], eax

    mov ebx, _l4_page_tbl
    mov ecx, _l3_page_tbl

    ; Enable PA.
    mov cr4, eax
    or eax, 1 << 5
    mov cr4, eax

    ; Set long mode bit.
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; "Yo CPU bro, we are using paging."
    mov eax, cr0
    or eax, (1 << 31) | 0x1
    mov cr0, eax

    lgdt [Pointer]
    jmp GDT64.Code:LM_START

GDT64:                           ; Global Descriptor Table (64-bit).
    .Null: equ $ - GDT64         ; The null descriptor.
    dw 0xFFFF                    ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 0                         ; Access.
    db 1                         ; Granularity.
    db 0                         ; Base (high).
    .Code: equ $ - GDT64         ; The code descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10011010b                 ; Access (exec/read).
    db 10101111b                 ; Granularity, 64 bits flag, limit19:16.
    db 0                         ; Base (high).
    .Data: equ $ - GDT64         ; The data descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10010010b                 ; Access (read/write).
    db 00000000b                 ; Granularity.
    db 0                         ; Base (high).
    .UserCode: equ $ - GDT64         ; The code descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 11111010b                 ; Access (exec/read).
    db 10101111b                 ; Granularity, 64 bits flag, limit19:16.
    db 0                         ; Base (high).
    .UserData: equ $ - GDT64         ; The data descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 11110010b                 ; Access (read/write).
    db 00000000b                 ; Granularity.
    db 0                         ; Base (high).
    Pointer:                    ; The GDT-pointer.
    dw $ - GDT64 - 1             ; Limit.
    dq GDT64    



bits 64
LM_START:
    mov ax, GDT64.Data
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    jmp POST_EXEC

POST_EXEC:
    hlt
    jmp POST_EXEC



align 4096                  ; Each entry is 8 bytes and there is
section .bss
_l4_page_tbl: resb 4096
_l3_page_tbl: resb 4096

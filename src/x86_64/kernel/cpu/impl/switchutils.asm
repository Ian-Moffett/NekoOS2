bits 32
global _lm_entry
global _context_switch

_lm_entry:

    ; Zero out page tables.
    mov edi, _l4_pge_tbl
    push edi                ; Save EDI.
    mov ecx, 1024
    xor eax, eax
    rep stosd

    mov edi, _l3_pge_tbl
    mov ecx, 1024
    rep stosd

    pop edi
    mov cr3, edi

    ; Setting flags.
    mov eax, _l3_pge_tbl
    add eax, 3 + (1 << 2)           ; Setting user bit.
    mov [edi], eax

    xor eax, eax
    mov eax, 3 | (1 << 7) | (1 << 2)
    mov [_l3_pge_tbl], eax

    mov ebx, _l4_pge_tbl
    mov ecx, _l3_pge_tbl

    ; Enable PAE.
    mov cr4, eax
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, (1 << 31) | 0x1
    mov cr0, eax

    lgdt [GDT64.Pointer]
    ; 0x08 is index in GDT.
    jmp 0x08:longmode_nya

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
    .UserCode: equ $ - GDT64     ; The code descriptor.
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
    db 0 
    .Pointer:                    ; The GDT-pointer.
    dw $ - GDT64 - 1             ; Limit.
    dq GDT64                     ; Base.



bits 64
extern _start
; We are now in long mode UwU.
longmode_nya: 
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax 
    call _start
    call _context_switch

post_exec:
    jmp post_exec

_context_switch:
    mov rsp, _context_switch_struct_nyaa
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsi
    pop rdi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax
    iretq

usermode_nyaa:
    jmp $

_context_switch_struct_nyaa:
    times 15 dq 0       ; Zeros out register space.
    dq usermode_nyaa
    dq 0x18 | 3         ; Bottom two bits for CS is CPL. (Current priv level).
    dq 0x0
    dq 0x10000          ; Usermode needs a new stack.
    dq 0x20 | 3         ; Requested priv level.


align 4096      ; addr % 4096 == 0.
_l4_pge_tbl: resb 4096
_l3_pge_tbl: resb 4096

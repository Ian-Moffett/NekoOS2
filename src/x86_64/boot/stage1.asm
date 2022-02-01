bits 16
org 0x7C00

xor ax, ax
mov ds, ax
mov es, ax
mov ss, ax

mov bp, 0x9C00
mov sp, bp

_start:
    mov [DRIVE_NUM], dl

    mov si, start_msg
    call puts

    mov si, start_msg1
    call puts

    mov si, boot_option
    call puts

    mov si, reboot_option
    call puts

    call read_keys

    mov ah, 0x0
    mov al, 0x2
    int 0x10

    mov si, boot_msg
    call puts

    ; Delay.
    mov cx, 0xF
    mov dx, 0x4240
    mov ah, 0x86
    int 0x15

    mov cx, 0xF
    mov dx, 0x4240
    mov ah, 0x86
    int 0x15

    mov dl, [DRIVE_NUM]
    call disk_load

    call 0x500     ; Pass control to stage 2.

DRIVE_NUM: db 0

read_keys:
    mov ah, 0x00
    int 0x16
    cmp al, 'r'
    je reboot
    cmp al, 'b'
    je rm_ret
    jmp read_keys

reboot: jmp 0xFFFF:0x0

puts:
    mov ah, 0x0E    ; Put char function.
    lodsb           ; Load byte from SI into AL and increment SI.
    or al, al       ; Check if AL is zero.
    jz rm_ret       ; If AL is zero return.
    int 0x10
    jmp puts

rm_ret: ret

dap:
    db 0x10     ; Size of DAP.
    db 0x0      ; Unused.
    dw 0x7      ; Sectors to read.
    dw 0x500    ; Dest buffer.
    dw 0x0      ; Dest segment.
    dq 0x1      ; Start sector.

disk_load:
    mov ah, 0x42
    mov si, dap
    int 0x13
    jc disk_error
    ret

disk_error:
    mov si, disk_error_msg
    call puts
    cli
    hlt


start_msg: db "You turned me on UwU <3", 0xD, 0xA, 0
start_msg1: db "Welcome to NekoOS Nyaa!~", 0xD, 0xA, 0
boot_option: db 0xD, 0xA, "[B]oot", 0xD, 0xA, 0
reboot_option: db "[R]eboot", 0xD, 0xA, 0
disk_error_msg: db "FATAL: Failed to read disk :(", 0
boot_msg: db "Booting NekoOS..", 0xD, 0xA, 0

; Sector padding to position magic at last
; two bytes.

times 510-($-$$) db 0
dw 0xAA55

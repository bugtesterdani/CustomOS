bits 16

section _ENTRY class=CODE

extern _cstart_
global init

; 56 50 53 AC ...
print_string:               dd 0x7D16
; BE 8B 7D E8 09 ...
wait_for_key_reboot:        dd 0x7d07

%define ENDL 0x0D, 0x0A

init:
    ; Is needed somewhy, to make the prints without bugs
    push ax
    pop ax
    mov si, load_msg
    call dword [print_string]

c_init:
    cli
    mov ax, ds
    mov ss, ax
    mov sp, 0
    mov bp, sp
    sti

    xor dh, dh
    push dx
    call _cstart_
    call dword [wait_for_key_reboot]

hlt:
    cli
    hlt

NEXT_OFFSET:    dd 0x1000

load_msg:                   db 'Loading stage2...', ENDL, 0
filename_stage2_part2:      db 'STAGE22 BIN'
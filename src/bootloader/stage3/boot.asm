bits 16
; org 0x8400

%define ENDL                    0x0D, 0x0A
; 56 50 53 AC ...
print_string:               dd 0x7D2A
; BE xx 7D E8 09 00 B4 ...
; BE xx 7C 01 E8 09 00 B4 ...
wait_for_key_reboot:        dd 0x7D1A
; F4 56 66 50 66 53 ...
read_fat32:                 dd 0x7CC3
; 10 00 02 00 ...
DAP:                        dd 0x7CF5
; 00 00 00 00 ...
var_32_zeros:               dd 0x7DCE


init:
    ; Is needed somewhy, to make the prints without bugs
    push ax
    pop ax
    mov si, msg
    call dword [print_string]
    pop si

hlt:
    cli
    hlt
    jmp hlt

msg:                        dd 'Hello World from Stage 3...', ENDL, 0
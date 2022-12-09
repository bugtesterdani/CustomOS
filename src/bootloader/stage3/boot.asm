bits 16
; org 0x8400

%define ENDL                    0x0D, 0x0A
; Stage1
; 56 50 53 AC ...
print_string:               dd 0x7D2A
; BE xx 7D E8 09 00 B4 ...
; BE xx 7C 01 E8 09 00 B4 ...
wait_for_key_reboot:        dd 0x7D1B
; F4 56 66 50 66 53 ...
read_fat32:                 dd 0x7CC3
; 10 00 02 00 ...
DAP:                        dd 0x7CF5
; 00 00 00 00 ...
var_32_zeros:               dd 0x7DCE

; Stage2
ReadFile:                   dd 0x8077
print_DAP_Values:           dd 0x8183
print_hex_word:             dd 0x81BC
set_in_dap_value:           dd 0x81DD


init:
    ; Is needed somewhy, to make the prints without bugs
    push ax
    pop ax
    mov si, msg
    call dword [print_string]
    pop si
    cli
    call Activate_A20_line
    call Load_GDT_Table
    call Start_Protected_Mode

hlt:
    cli
    hlt
    jmp hlt

msg:                        dd 'Hello World from Stage 3...', ENDL, 0


; Following Code is in 32-bit
; This is for the protected code

bits 32
Activate_A20_line:
.check_A20_line:
    pushad
    mov edi, 0x112345               ; odd megabyte address.
    mov esi, 0x012345               ; even megabyte address.
    mov [esi], esi                  ; making sure that both addresses contain different values.
    mov [edi], edi                  ; (if A20 line is cleared the two pointers would point to the address 0x012345 that would contain 0x112345 (edi))
    cmpsd                           ; compare addresses to see if they are equivalent.
    popad
    jne .isActive
    jmp .activate

.activate:
    cli                             ; Disable Interrupts

    call .a20wait
    mov al, 0xAD
    out 0x64, al

    call .a20wait
    mov al, 0xD0
    out 0x64, al
    
    call a20wait2
    in al, 0x60
    push eax

    call a20wait
    mov al, 0xD1
    out 0x64, al

    call a20wait
    pop eax
    or al, 2
    out 0x60, al
    
    call a20wait
    mov al, 0xAE
    out 0x64, al

    call a20wait
    sti                             ; Reset Interrupts
    ret

.isActive:
    ret

.a20wait:
    in al, 0x64
    test al, 2
    jnz, .a20wait
    ret

.a20wait2:
    in al, 0x64
    test al, 1
    jz .a20wait2
    ret

Load_GDT_Table:
    ret

Start_Protected_Mode:
    ; We need to modify the CR0 Register (Control Register 0)
    mov eax, cr0
    or al, 1        ; Set PE (Protection Enable) bit (lowest bit (2^0)) in CR0 (Control Register 0)
    ; Then we need to store it at the Register back
    mov cr0, eax
    ret
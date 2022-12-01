bits 16

section _TEXT class=CODE

; 56 50 53 AC ...
print_string:               dd 0x7D16
; B4 41 BB AA ...
read_fat32:                 dd 0x7CC3
; 10 00 01 00 ...
DAP:                        dd 0x7CE3

global _x86_PrintString
_x86_PrintString:
    push bp
    mov bp, sp
    push bx

    push si
    push di
    mov di, bp
    mov si, [di + 4]
    pop di
    call dword [print_string]
    pop si

    pop bx
    mov sp, bx
    pop bp
    ret

global _x86_waitKeyPress
_x86_waitKeyPress:
    push bp
    mov bp, sp
    push bx

    push ax
    mov ah, 0               ; AH Set to 0 makes an read keystroke Interrupt by called Int 16h
                            ; Read Key Press
    int 16h                 ; Keystroke on the Keyboard Interrupt
    pop ax
    
    pop bx
    mov sp, bp
    pop bp
    ret

global _x86_ReadFAT
_x86_ReadFAT:
    push bp
    mov bp, sp
    push bx

    push di
    mov di, bp
    push si
    mov si, [DAP + 2]
    mov si, [di + 4]     ; Amount of Sectors to read
    mov si, [DAP + 8]
    mov si, [di + 6]     ; LSB LBA1
    mov si, [DAP + 9]
    mov si, [di + 8]     ; MSB LBA1
    mov si, [DAP + 10]
    mov si, [di + 10]   ; LSB LBA2
    mov si, [DAP + 11]
    mov si, [di + 12]   ; MSB LBA2
    mov si, [DAP + 12]
    mov si, [di + 14]   ; LSB LBA3
    mov si, [DAP + 13]
    mov si, [di + 16]   ; MSB LBA3
    mov si, [DAP + 14]
    mov si, [di + 18]   ; LSB LBA4
    mov si, [DAP + 15]
    mov si, [di + 20]   ; MSB LBA4
    mov si, [DAP + 5]
    mov si, [di + 22]    ; LSB BUFF OFFSET
    mov si, [DAP + 6]
    mov si, [di + 24]    ; MSG BUFF OFFSET
    pop si
    pop di
    push si
    mov si, [DAP + 2]
    call dword [print_string]
    pop si
    ;call dword [read_fat32]
    mov eax, DAP

    pop bx
    mov sp, bx
    pop bp
    ret
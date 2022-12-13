bits 16
;org 0x8000

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

Result_Read_File:           dd 0x0

%define ENDL                    0x0D, 0x0A
%define ROOT_DIR_START          0x0B18
%define ROOT_DIR_START_LSB      0x18
%define ROOT_DIR_START_MSB      0x0B
%define SECTORS_PER_CLUSTER     8
%define Buff_Off                0x8400

init:
    ; Is needed somewhy, to make the prints without bugs
    push ax
    pop ax
    mov si, load_msg
    call dword [$stage1.print_string]
    pop ax

; What do we need to do?
; - Read the File stage22.bin into the Memory (located from 0x1000)
; - Read all Parts and then jump to this location.
; - Define more usefull Functions, which could be used in stage2 later
;   (saving space)
start:
    ; STACK => AX, CX, DX, BX, SP, BP, SI, DI (first -> last)
    pusha
.assign_values:
    ; Assign DAP Values
    xor ax, ax
    ; Amount of Sectors to read
    mov al, 2
    mov ah, 1
    call set_in_dap_value
    ; LSB LBA 1
    mov al, 8
    mov ah, ROOT_DIR_START_LSB
    call set_in_dap_value
    ; MSB LBA 1
    mov al, 9
    mov ah, ROOT_DIR_START_MSB
    call set_in_dap_value
    mov al, 4
    mov ah, 0x00
    call set_in_dap_value
    ; MSB Buffer Offset
    mov al, 5
    mov ah, 0x84
    call set_in_dap_value
    ; LSB Buffer Offset
    mov al, 6
    mov ah, 0x00
    call set_in_dap_value

    ; continue from start
    mov si, filename_stage3
    call ReadFile
    mov si, Result_Read_File
    mov ax, 1
    xor [si], ax
    jz file_not_found
    call LoadFile
    jmp Execute

Execute:
    popa
    call print_DAP_Values
    jmp dword Buff_Off

hlt:
    cli
    hlt

Line:               db ' - ', 0

; Parameter si sollte der Dateiname eingesetzt werden.
; Return Value: 0 => Success, 1 => Fail
; Value of the Readed Array, is in register di
ReadFile:
.read_fat32_interrupt:
    call dword [$stage1.read_fat32]
    pop ax
    ; Backup si
    mov ax, si
.compare_file_name:
    mov si, ax
    mov cx, 11
    push di
    repe cmpsb
    pop di
    je .found_file
.check_if_directory:
    ; look if it is a directory
.check_long_filename:
    ; look if it is a long name
    ; check for this the Byte mit offset 0xB
.check_if_only_zeros:
    ; This means the end of the Table
    add di, 020h
    mov si, [$stage1.var_32_zeros]
    mov cx, 11
    push di
    repe cmpsb
    pop di
    je .file_not_found
    jmp .compare_file_name
.found_file:
    mov si, msg_file_found
    call dword [$stage1.print_string]
    pop si
    mov si, Result_Read_File
    mov ax, 0
    xor [si], ax
    ret
.file_not_found:
    mov si, [Result_Read_File]
    pop si
    mov si, Result_Read_File
    mov ax, 0
    xor [si], ax
    ret

LoadFile:
.loading_file:
    and eax, 0x00000000
    and ebx, 0x00000000
    and ecx, 0x00000000
    mov ecx, SECTORS_PER_CLUSTER        ; => Sectors Per Cluster
    mov eax, [di + 0x14]
    shl eax, 8
    add eax, [di + 0x1A]
    dec eax
    dec eax                             ; => 0x0000 0001
    mul ecx                             ; => 0x0000 2008

    add ax, ROOT_DIR_START              ; Default Hex Value where the Root Directory starts
    mov ebx, eax
    xor eax, eax
    mov al, 8
    mov ah, bl
    call set_in_dap_value

    mov al, 9
    mov ah, bh
    call set_in_dap_value

    ; Calculate how many Sectors we should be reading
    and eax, 0x00000000
    and ebx, 0x00000000
    and ecx, 0x00000000
    and edx, 0x00000000
    mov ebx, [di + 0x1F]
    and ebx, 0x000000FF
    mov eax, ebx
    shl eax, 8
    mov ebx, [di + 0x1E]
    and ebx, 0x000000FF
    add eax, ebx
    shl eax, 8
    mov ebx, [di + 0x1D]
    and ebx, 0x000000FF
    add eax, ebx
    shl eax, 8
    mov ebx, [di + 0x1C]
    and ebx, 0x000000FF
    add eax, ebx
    mov cx, 0x0200
    div cx
    inc eax

    mov ebx, eax
    and eax, 0x00000000
    and ecx, 0x00000000
    mov al, 2
    mov ah, bl
    call set_in_dap_value

    and eax, 0x00000000
    mov al, 3
    mov ah, bh
    call set_in_dap_value

    call print_DAP_Values

    call dword [$stage1.read_fat32]
    pop si
.finish_reading:
    mov si, load_msg
    call dword [$stage1.print_string]
    pop si
    ret

print_DAP_Values:
    push ax
    mov ax, 16
.testing_print_hex_values:
    mov si, [$stage1.DAP]
    sub si, ax
    mov si, [si + 16]
    push si
    call print_hex_word
    mov si, Line
    call dword [$stage1.print_string]
    pop si
    pop si
    dec ax
    dec ax
    jnz .testing_print_hex_values
    mov si, msg_new_line
    call dword [$stage1.print_string]
    pop ax                              ; First remove the additional byte, we dont need.
                                        ; This byte comes from the dword call
    pop ax
    ret

msg_new_line:       db ENDL, 0

file_not_found:
    mov si, msg_file_not_found
    call dword [$stage1.print_string]
    pop si
    jmp hlt

print_hex_word:
    pusha
    mov bp, sp
    mov cx, 0x0404
    mov dx, [bp+18]
    mov bx, [bp+20]
.loop:
    rol dx, cl
    mov ax, 0x0e0f
    and al, dl
    add al, 0x90
    daa
    adc al, 0x40
    daa
    int 0x10
    dec ch
    jnz .loop
    popa
    ret

set_in_dap_value:
.start:
    push si
.setup_loop:
    mov si, [$stage1.DAP]
.loop:
    inc si
    dec al
    test al, al
    jnz .loop
    mov [si], ah
.end:
    pop si
    ret

load_msg:                   db 'Loading Bootloader Part 2...', ENDL, 0
msg_file_not_found:         db 'Bootloader Part 2 could not be found', ENDL, 0
msg_file_found:             db 'Found Bootloader Part 2. Loading...', ENDL, 0
filename_stage3:            db 'STAGE3  BIN'
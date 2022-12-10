bits 16
; org 0x8400

section .entry
global entry

%define ENDL 0x0D, 0x0A
%define Black 0x0
%define Blue 0x1
%define Green 0x2
%define Cyan 0x3
%define Red 0x4
%define Purple 0x5
%define Brown 0x6
%define Gray 0x7
%define Dark_Gray 0x8
%define Light_Blue 0x9
%define Light_Green 0xA
%define Light_Cyan 0xB
%define Light_Red 0xC
%define Light_Purple 0xD
%define Yellow 0xE
%define White 0xF

entry:
    cli
    lgdt [gdt_desc]
    mov eax, cr0
    or al, 1
    mov cr0, eax
    jmp CODE_SEG:init
    jmp halt
halt:
    hlt
    jmp halt

bits 32
init:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ebp, 0x90000
    mov esp, ebp
    mov bl, Black
    shl bl, 4
    add bl, White
    mov eax, 0                      ; Print to the 1. Line
    mov si, print_string            ; Load the String we want to print
    call print_to_video_card        ; Call the print function
    mov bl, Black
    shl bl, 4
    add bl, Yellow
    mov eax, 1                      ; Print to the 2. Line
    mov si, print_string_second
    call print_to_video_card
    ;call _cstart_
hang:
    jmp hang

bits 32
print_to_video_card:
    push si
    mov ecx, 0x50
    mul ecx
    mov ecx, 2
    mul ecx
    mov edx, 0xB8000
    add edx, eax
    sub edx, 1
.loop:
    lodsb
    or al, al
    jz .done
.print_char:
    add edx, 1
    mov byte [ds:edx], al
    add edx, 1
    mov byte [ds:edx], bl
    jmp .loop
.done:
    pop si
    ret

print_string:           db 'Protected Mode', 0
print_string_second:    db 'Welcome to DP OS', 0



; Following the GDT Descriptions

bits 16
; Global Descriptor Tables
;  1st Double word:
;  Bits	            Function            Description
;  0-15             Limit 0:15	        First 16 bits in the segment limiter
;  16-31            Base 0:15	        First 16 bits in the base address
;
;
;  2nd Double word:
;  Bits	            Function            Description
;  0-7              Base 16:23	        Bits 16-23 in the base address
;  8-12             Type	            Segment type and attributes
;  13-14            Privilege
;           Level 0 = Highest privilege (OS), 
;           Level 3 = Lowest privilege (User applications)
;  15	            Present flag	    Set to 1 if segment is present
;  16-19            Limit 16:19         Bits 16-19 in the segment limiter
;  20-22            Attributes          Different attributes, depending on the segment type
;  23	            Granularity
;           Used together with the limiter, to determine the size of the segment
;  24-31            Base 24:31          The last 24-31 bits in the base address

; The GDT Descriptor
;  Bits             Function            Description
;  0-15             Limit               Size of GDT in bytes
;  16-47            Address             GDT's memory address

; Memory Map to 0xFFFFF
;  Linear address range (hex)	Memory type	Use
;  0 - 3FF          RAM                 Real mode, IVT (Interrupt Vector Table)
;  400 - 4FF        RAM                 BDA (BIOS data area)
;  500 - 9FFFF      RAM                 Free memory, 7C00 used for boot sector
;  A0000 - BFFFF	Video RAM           Video memory
;  C0000 - C7FFF	Video ROM           Video BIOS
;  C8000 - EFFFF	?                   BIOS shadow area
;  F0000 - FFFFF    ROM                 System BIOS

gdt_start:

gdt_null:
    dq 0

gdt_code:
    ; 1. Double word
    dw 0FFFFh           ; 4GB Limit
    dw 0                ; Offset / Base Address set to 0
    ; 2. Double word
    db 0                ; Base Address 8 16-23 bit
    db 10011010b
    db 11001111b
    db 0

gdt_data:
    ; 1. Double word
    dw 0FFFFh           ; 4GB Limit
    dw 0                ; Offset / Base Address set to 0
    ; 2. Double word
    db 0                ; Base Address 8 16-23 bit
    db 10010010b
    db 11001111b
    db 0

gdt_end:

gdt_desc:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

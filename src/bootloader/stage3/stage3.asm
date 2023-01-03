bits 16
; org 0x8400

section .entry
global entry
extern _cstart_

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

StackPointer_Value:         dd 0x0

entry:
.clear_registers_0500_0900:
    ; Setze den Wert von BX auf 0x500
    mov bx, 0x500
    ; Setze den Wert von CX auf 0x400 (0x900 - 0x500)
    mov cx, 0x400
    ; Fülle die Register von 0x500 bis 0x900 mit dem Wert 0x00
.fillloop:
    mov byte [bx], 0x00 ; Fülle das aktuelle Register mit 0x00
    inc bx ; Inkrementiere BX
    dec cx ; Decrementiere CX
    jnz .fillloop ; Wiederhole die Schleife, solange CX nicht 0 ist
.calc_sp:
    mov ecx, 0x400
    mov eax, [0x413]
    mul ecx
    mov [StackPointer_Value], eax
    mov sp, ax
.load_ram_part:
    call do_e820
; .change_vga_resolution:
;     mov ah, 0x00
;     mov al, 0x12
;     int 0x10
.change_to_protected_mode:
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
    mov ebp, [StackPointer_Value]
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
    ;call read_input
    call _cstart_
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


bits 16
; use the INT 0x15, eax= 0xE820 BIOS function to get a memory map
; note: initially di is 0, be sure to set it to a value so that the BIOS code will not be overwritten. 
;       The consequence of overwriting the BIOS code will lead to problems like getting stuck in `int 0x15`
; inputs: es:di -> destination buffer for 24 byte entries
; outputs: bp = entry count, trashes all registers except esi
mmap_ent equ 0x0500             ; the number of entries will be stored at 0x8000
do_e820:
    mov di, 0x0504          ; Set di to 0x8004. Otherwise this code will get stuck in `int 0x15` after some entries are fetched 
	xor ebx, ebx		; ebx must be 0 to start
	xor bp, bp		; keep an entry count in bp
	mov edx, 0x0534D4150	; Place "SMAP" into edx
	mov eax, 0xe820
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24		; ask for 24 bytes
	int 0x15
	jc short .failed	; carry set on first call means "unsupported function"
	mov edx, 0x0534D4150	; Some BIOSes apparently trash this register?
	cmp eax, edx		; on success, eax must have been reset to "SMAP"
	jne short .failed
	test ebx, ebx		; ebx = 0 implies list is only 1 entry long (worthless)
	je short .failed
	jmp short .jmpin
.e820lp:
	mov eax, 0xe820		; eax, ecx get trashed on every int 0x15 call
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24		; ask for 24 bytes again
	int 0x15
	jc short .e820f		; carry set means "end of list already reached"
	mov edx, 0x0534D4150	; repair potentially trashed register
.jmpin:
	jcxz .skipent		; skip any 0 length entries
	cmp cl, 20		; got a 24 byte ACPI 3.X response?
	jbe short .notext
	test byte [es:di + 20], 1	; if so: is the "ignore this data" bit clear?
	je short .skipent
.notext:
	mov ecx, [es:di + 8]	; get lower uint32_t of memory region length
	or ecx, [es:di + 12]	; "or" it with upper uint32_t to test for zero
	jz .skipent		; if length uint64_t is 0, skip entry
	inc bp			; got a good entry: ++count, move to next storage spot
	add di, 24
.skipent:
	test ebx, ebx		; if ebx resets to 0, list is complete
	jne short .e820lp
.e820f:
	mov [mmap_ent], bp	; store the entry count
	clc			; there is "jc" on end of list to this point, so the carry must be cleared
	ret
.failed:
	stc			; "function unsupported" error exit
	ret


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

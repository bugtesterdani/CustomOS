; org 0x0x00
bits 32

; Im Paging Mode ESP
nop
nop
nop
nop

mov ebx, 0x0
mov eax, 0xbf000000
mov [ebx], eax

; We will need to initialize paging and than jmp to the address which is located in eax

mov ebx, [ebp + 0xc]

enablePaging:
    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax

mov ebp, 0x32
mov [ebp], esp
mov ebp, 0x0
mov esp, [ebp]
; mov ebp, esp
jmp ebx
ret

nop

; Im normal Mode ESP
nop
nop
nop
nop

nop
nop
nop
nop

pusha
mov ebp, 0x0
mov [ebp], esp

disablePaging:
    mov eax, cr0
    and eax, 0x7FFFFFFF
    mov cr0, eax

mov ebp, 0x32
mov esp, [ebp]
int 0x80
mov ebp, 0x11
jmp ebp

nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
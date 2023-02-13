; org 0x0x00
bits 32

; We will need to initialize paging and than jmp to the address which is located in eax

enablePaging:
    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax

mov eax, 0xbf000000
jmp eax
ret

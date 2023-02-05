; org 0x0x00

%define STAGE4_KERNEL 0xFF000000
; We will need to initialize paging and than jmp to the address which is located in eax

enablePaging:
    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax

call STAGE4_KERNEL
ret

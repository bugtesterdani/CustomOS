[bits 32]

global loadPageDirectory:
loadPageDirectory:
    mov eax, [esp + 4]
    mov cr3, eax
    ret

global enablePaging:
enablePaging:
    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax
    ret

global disablePaging:
disablePaging:
    mov eax, cr0
    btr eax, 31
    mov cr0, eax
    ret

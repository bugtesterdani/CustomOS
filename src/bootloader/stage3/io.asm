[bits 32]

global io_outportb:
io_outportb:
    mov edx, [esp + 4]
    mov eax, [esp + 8]
    out dx, al
    ret

global io_inportb:
io_inportb:
    mov edx, [esp + 4]
    in al, dx
    ret

global io_outportw:
io_outportw:
    mov edx, [esp + 4]
    mov eax, [esp + 8]
    out dx, ax
    ret

global io_inportw:
io_inportw:
    mov edx, [esp + 4]
    in ax, dx
    ret

global io_outportd:
io_outportd:
    mov edx, [esp + 4]
    mov eax, [esp + 12]
    out dx, eax
    ret

global io_inportd:
io_inportd:
    mov edx, [esp + 4]
    in eax, dx
    ret
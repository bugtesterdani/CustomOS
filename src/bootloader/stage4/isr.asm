[bits 32]

extern isr_handler
extern irq_handler

; cpu pushes to the stack: ss, esp, eflags, cs, eip

%macro ISR_NOERRORCODE 1

global ISR%1:
ISR%1:
    cli
    push 0              ; push dummy error code
    push %1             ; push interrupt number
    jmp isr_common
%endmacro

%macro ISR_ERRORCODE 1
global ISR%1:
ISR%1:
    cli
                        ; cpu pushes an error code to the stack
    push %1             ; push interrupt number
    jmp isr_common
%endmacro

%macro IRQHandler 2
global IRQ%1:
IRQ%1:
    cli
    push byte 0
    push byte %2
    jmp irq_common
%endmacro

global stop_system:
stop_system:
    cli
    hlt
    jmp stop_system

global asm_enable_interrupts:
asm_enable_interrupts:
    sti
    ret

%include "inc/isr.inc"
%include "inc/irq.inc"

global asm_syscall_success:
asm_syscall_success:
    push eax
    push ebx

    mov eax, esp
    add eax, 0x9c
    mov ebx, [eax]
    add ebx, 0x2
    mov [eax], ebx

    pop ebx
    pop eax
    ret

isr_common:
    pusha               ; pushes in order: eax, ecx, edx, ebx, esp, ebp, esi, edi
    cli
    cld

    xor eax, eax        ; push ds
    mov ax, ds
    push eax

    mov ax, 0x10        ; use kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    push esp            ; pass pointer to stack to C, so we can access all the pushed information
    call isr_handler
    add esp, 4

    pop eax             ; restore old segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                ; pop what we pushed with pusha
    add esp, 8          ; remove error code and interrupt number
    sti
    iret                ; will pop: cs, eip, eflags, ss, esp

irq_common:
    pusha
    cli
    cld

    xor eax, eax
    mov ax, ds
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call irq_handler
    add esp, 4

    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa
    add esp, 8
    sti
    iret
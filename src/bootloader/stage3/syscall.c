#include "headers/syscall.h"
#include "headers/idt.h"
#include "headers/isr.h"
#include "headers/stdio.h"
#include "headers/colors.h"

void asm_functions(syscall_ISR());

void syscall_init()
{
    idt_set_gate(0x80, syscall_ISR, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING3 | IDT_FLAG_GATE_32BIT_INT);
    idt_enable_gate(0x80);
}

int32_t sys_write(uint32_t fd, const char* buffer, uint32_t count)
{
    if (fd != 1 || buffer == 0)
    {
        return -1;
    }
    for (uint32_t i = 0; i < count; i++)
    {
        outputchar(buffer[i], White, Black);
    }
    return (int32_t)count;
}

void syscall_dispatch(registers_t *regs)
{
    switch (regs->eax)
    {
        case SYS_WRITE:
            regs->eax = (uint32_t)sys_write(regs->ebx, (const char*)regs->ecx, regs->edx);
            break;
        default:
            regs->eax = (uint32_t)-1;
            break;
    }
}

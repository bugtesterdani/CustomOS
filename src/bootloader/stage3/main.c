#include "headers/stdio.h"
#include "headers/stdint.h"
#include "headers/screen.h"
#include "headers/colors.h"
#include "headers/gdt.h"
#include "headers/idt.h"
#include "headers/isr.h"
#include "headers/RAM.h"
#include "headers/memory_management.h"
#include "headers/fat32.h"
#include "headers/initialize.h"
#include "headers/paging.h"
#include "headers/elf.h"
#include "headers/syscall.h"

#define SYSTEM_MEMORY   2 * 4096            // Request 4096 Bit. We will need to change this later, when more needed.
#include "headers/offset_List.h"

void testing_idt();

void _cstart_()
{
    RAM_FullInit();
    uint32_t address = 0;
    uint32_t offset = 300;
    if (allocate_block(&address, &offset, SYSTEM_MEMORY) == 0)
    {
        stopping_system();
    }
    InitializeAddresses(address + offset);
    initScreen();
    init_gdt();
    isr_init();
    irq_init();
    syscall_init();
    init_idt();
    enable_interrupts();

    testing_idt();

    // syscall verification
    const char *sys_msg = "[int80] syscall write ok";
    __asm__ __volatile__("int $0x80" : : "a"(SYS_WRITE), "b"(1), "c"(sys_msg), "d"(24));

    paging_setup();
    Parsing("KERNEL.ELF");

    // We will switch before to stage 4
    for (;;);
}

struct idt
{
    void *base;
    unsigned short length;
};

void testing_idt()
{
    struct idt idt;
    __asm__ __volatile__ ("sidt %0" : "=m"(idt));
}

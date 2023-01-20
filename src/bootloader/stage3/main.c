#include "headers/stdio.h"
#include "headers/stdint.h"
#include "headers/screen.h"
#include "headers/keyboard.h"
#include "headers/colors.h"
#include "headers/commands.h"
#include "headers/gdt.h"
#include "headers/idt.h"
#include "headers/isr.h"
#include "headers/RAM.h"
#include "headers/monitor.h"
#include "headers/memory_management.h"
#include "headers/fat32.h"
#include "headers/initialize.h"
#include "headers/paging.h"

#define SYSTEM_MEMORY   2 * 4096            // Request 4096 Bit. We will need to change this later, when more needed.
#include "headers/offset_List.h"

void _cstart_()
{
    char Buff[60];

    RAM_FullInit();
    uint32_t address = 0;
    uint32_t offset = 0;
    if (allocate_block(&address, &offset, SYSTEM_MEMORY) == 0)
    {
        // printString("Memory allocation failed in main thread", White, Black);
        // setcursornewline();
        // endless loop
        stopping_system();
    }
    InitializeAddresses(address + offset);
    initScreen();
    init_gdt();
    isr_init();
    irq_init();
    init_idt();
    enable_interrupts();
    paging_setup();

#ifdef VGA_TEST
    setupmode(320, 200, 256);
    for (uint32_t x = 0; x < 320; x++)
    {
        for (uint32_t y = 0; y < 200; y++)
        {
            putpixel(x, y, 0x01);
        }
    }

    DrawRectAngle(10, 10, 300, 180, 1, 0x34);
    FillRectangle(20, 20, 30, 30, 0x04);
    FillRectangle(80, 20, 30, 30, 0x0A);
    FillRectangle(80, 80, 30, 30, 0x0E);
#endif

#ifdef ALLOCATE_TEST
    uint8_t output[80];
    uint32_t address = 0;
    uint32_t offset = 0;
    uint8_t _lastindex = 0;
    clearArray(Buff, 60, 0x00);
    ReadLine(Buff, 60);
    for (uint32_t i = 0; i < 785; i++)
    {
        address = 0;
        offset = 0;

        if (allocate_block(&address, &offset, 1000 * 4096) == 0)
        {
            clearArray(output, 80, 0x00);
            ConvertToChar(i, 10, output, 0);
            printString(output, Yellow, Black);
            printString(" ERROR: Failed to allocate", White, Black);
            setcursornewline();
            break;
        }

        clearArray(output, 80, 0x00);
        ConvertToChar((address >> 16) & 0xFFFF, 16, output, 0);
        _lastindex = lastIndex(output, 80);
        output[_lastindex] = ' ';
        ConvertToChar((address >>  0) & 0xFFFF, 16, output, _lastindex + 1);
        _lastindex = lastIndex(output, 80);
        output[_lastindex] = ' ';
        ConvertToChar((offset >> 16) & 0xFFFF, 16, output, _lastindex + 1);
        _lastindex = lastIndex(output, 80);
        ConvertToChar((offset >>  0) & 0xFFFF, 16, output, _lastindex);
        _lastindex = lastIndex(output, 80);
        printString(output, Yellow, Black);
        setcursornewline();
    }
#endif

    while (1)
    {
        clearArray(Buff, 60, 0x00);
        printString("> ", White, Black);
        setcursorX(2);
        ReadLine(Buff, 60);
        setcursornewline();
        ParseCommand(Buff);
        setcursornewline();
    }

    // endless loop
    for (;;);
}
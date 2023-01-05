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

void _cstart_()
{
    initScreen();
    RAM_FullInit();
    init_gdt();
    isr_init();
    irq_init();
    init_idt();
    enable_interrupts();

#ifdef SetVGAMode
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

    char Buff[60];
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
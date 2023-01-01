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

void _cstart_()
{
    initScreen();
    RAM_FullInit();
    init_gdt();
    isr_init();
    irq_init();
    init_idt();
    enable_interrupts();
    printString("Hello from C Code", White, Black);
    setcursornewline();
    printString("Hello World", White, Black);
    setcursornewline();

    while (1)
    {
        char Buff[60];
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
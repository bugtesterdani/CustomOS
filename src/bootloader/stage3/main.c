#include "headers/stdio.h"
#include "headers/stdint.h"
#include "headers/screen.h"
#include "headers/keyboard.h"
#include "headers/colors.h"
#include "headers/commands.h"

void _cstart_()
{
    initScreen();
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
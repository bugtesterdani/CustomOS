#include "headers/stdio.h"
#include "headers/stdint.h"
#include "headers/screen.h"
#include "headers/util.h"
#include "headers/keyboard.h"
#include "headers/colors.h"
#include "headers/commands.h"

void _cstart_()
{
    initScreen();
    print("Hello from C Code", 3, White, Black);
    setcursor(0, 5);
    printString("Hello World", White, Black);
    setcursor(0,7);

    while (1)
    {
        char Buff[60];
        ReadLine(Buff, 60);
        setcursor(0,8);
        ParseCommand(Buff);
        setcursor(0,7);
    }


    // endless loop
    for (;;);
}
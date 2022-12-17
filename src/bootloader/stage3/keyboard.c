#include "headers/keyboard.h"
#include "headers/io.h"
#include "headers/colors.h"
#include "headers/stdio.h"

// ASCII TABLE
//      0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
// --------------------------------------------------------------------
// 0   NUL SOH STX ETX EOT ENQ ACK BEL BS  HT  LF  VT  FF  CR  SO  SI
// 1   DLE DC1 DC2 DC3 DC4 NAK SYN ETB CAN EM  SUB ESC FS  GS  RS  US
// 2    SP  !   "   #   $   %   &   '   (   )   *   +   ,   -   .   /
// 3    0   1   2   3   4   5   6   7   8   9   :   ;   <   =   >   ?
// 4    @   A   B   C   D   E   F   G   H   I   J   K   L   M   N   O
// 5    P   Q   R   S   T   U   V   W   X   Y   Z   [   \   ]   ^   _
// 6    `   a   b   c   d   e   f   g   h   i   j   k   l   m   n   o
// 7    p   q   r   s   t   u   v   w   x   y   z   {   |   }   ~ DEL

unsigned int FetchScancode()
{
    // port 0x60 -> scancode + shift key -> ASCII
    return( inportb(0x60)); // get scan code from the keyboard
}

int ShiftKeyDown;

unsigned int FetchAndAnalyzeScancode()
{
    unsigned int scancode;
    while(1) // Loop until a key to be pressed
    {
        // Wait for the key
        while ( !(inportb(0x64)&1) ); // 0x64: read keyboard µC status register
        scancode = FetchScancode();

        if ( scancode & 0x80 ) // Key released? Check bit 7 (10000000b = 0x80) of scan code for this
        {
            scancode &= 0x7F; // Key was released, compare only low seven bits: 01111111b = 0x7F
            if ( scancode == KRLEFT_SHIFT || scancode == KRRIGHT_SHIFT ) // A key was released, shift key up?
                ShiftKeyDown = 0x00;
            continue;
        }

        if ( scancode == KRLEFT_SHIFT || scancode == KRRIGHT_SHIFT )
        {
            ShiftKeyDown = 0x80;
            continue;
        }
        return scancode | ShiftKeyDown;
    }
}

void ReadLine(char* buff, unsigned int length)
{
    unsigned char KeyGot=0;
    unsigned int counter = 0;
    KeyGot = FetchAndAnalyzeScancode();
    while (KeyGot != 28)
    {
        char printstr[2];
        char variable = asciiShift[KeyGot];
        if ((KeyGot & 0x80) == 0x80)
        {
            variable = asciiNonShift[KeyGot];
        }
        switch (parseKey(variable))
        {
            case 0:
                printstr[0] = asciiNonShift[KeyGot];
                buff[counter] = asciiNonShift[KeyGot];
                printString(printstr, White, Black);
                counter++;
                if (counter == length)
                {
                    return;
                }
                break;
            case 1:
                if (counter != 0)
                {
                    counter--;
                    buff[counter] = 0;
                }
                break;
            default:
                break;
        }

        KeyGot = FetchAndAnalyzeScancode();
    }
}

unsigned int parseKey(char Key)
{
    switch (Key)
    {
        case BACKSPACE:
            removeLastCharacter();
            return 1;
        default:
            return 0;
    }
}

#include "headers/keyboard_tmp.h"
#include "headers/io.h"
#include "headers/colors.h"
#include "headers/stdio.h"
#include "headers/screen.h"
#include "headers/isr.h"

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

unsigned int tmp_FetchScancode()
{
    // port 0x60 -> scancode + shift key -> ASCII
    return( inportb(0x60)); // get scan code from the keyboard
}

int tmp_ShiftKeyDown;

unsigned int tmp_FetchAndAnalyzeScancode()
{
    unsigned int scancode;
    while(1) // Loop until a key to be pressed
    {
        // Wait for the key
        while ( !(inportb(0x64)&1) ); // 0x64: read keyboard µC status register
        scancode = tmp_FetchScancode();

        if ( scancode & 0x80 ) // Key released? Check bit 7 (10000000b = 0x80) of scan code for this
        {
            scancode &= 0x7F; // Key was released, compare only low seven bits: 01111111b = 0x7F
            if ( scancode == KRLEFT_SHIFT || scancode == KRRIGHT_SHIFT ) // A key was released, shift key up?
                tmp_ShiftKeyDown = 0x00;
            continue;
        }

        if ( scancode == KRLEFT_SHIFT || scancode == KRRIGHT_SHIFT )
        {
            tmp_ShiftKeyDown = 0x80;
            continue;
        }
        return scancode | tmp_ShiftKeyDown;
    }
}

static void tmp_keyboard_callback(registers_t* regs)
{
    char KeyGot = 0;
    if (inportb(0x64) & 0x01)
    {
        KeyGot = tmp_ReadChar();
    }
    if (KeyGot != 28)
    {
        char output[2];
        output[0] = KeyGot;
        printString(output, White, Black);
    }
}

void tmp_keyboard_init()
{
    uint8_t newvalue = inportb(0x21) & ~(IQ_KEYBOARD - IRQ_OFFSET);
    outportb(0x21, newvalue);
    enable_interrupts();
}

void rtmp_egister_keyboard_callback()
{
    tmp_keyboard_init();
    irq_register(IQ_KEYBOARD, &tmp_keyboard_callback);
    printString("Key Interrupt registered", White, Black);
}

char tmp_ReadChar()
{
    unsigned char KeyGot = tmp_FetchAndAnalyzeScancode();
    if ((KeyGot & 0x7F) == 28)
    {
        return KeyGot;
    }
    char variable = asciiNonShift[KeyGot];
    if ((KeyGot & 0x80) == 0x80)
    {
        variable = asciiShift[KeyGot];
    }
    return variable;
}

void tmp_ReadLine(char* buff, unsigned int length)
{
    unsigned char KeyGot = tmp_ReadChar();
    unsigned int counter = 0;
    while ((KeyGot & 0x7F) != 28)
    {
        char printstr[2];
        switch (tmp_parseKey((KeyGot & 0x7F)))
        {
            case 0:
                printstr[0] = KeyGot;
                buff[counter] = KeyGot;
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

        KeyGot = tmp_ReadChar();
    }
}

unsigned int tmp_parseKey(char Key)
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

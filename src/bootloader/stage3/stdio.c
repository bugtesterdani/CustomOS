#include "headers/stdio.h"
#include "headers/x86.h"

void print(const char* str, uint8_t Line, uint8_t Foreground, uint8_t Background)
{
    uint8_t Character = 0;
    while (*str)
    {
        putchar(*str, Character, Line, Foreground, Background);
        Character++;
        str++;
    }
}

void putchar(char character, uint8_t Row, uint8_t Line, uint8_t Foreground, uint8_t Background)
{
    x86_PM_Video_WriteCharTeletype(character, Row, Line, Foreground, Background);
}

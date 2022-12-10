#include "headers/stdio.h"
#include "headers/screen.h"

void print(const char* str, uint8_t Line, uint8_t Foreground, uint8_t Background)
{
    uint8_t Character = 0;
    while (*str)
    {
        outputchar(*str, Character, Line, Foreground, Background);
        Character++;
        str++;
    }
}

void initScreen()
{
    clearscreen();
}


void outputchar(char character, uint8_t Row, uint8_t Line, uint8_t Foreground, uint8_t Background)
{
    uint8_t color = Background << 4;
    color |= Foreground;
    putcharp(Row, Line, character, (Background << 4) | Foreground);
}

#include "headers/stdio.h"
#include "headers/screen.h"

void print(const char* str, uint8_t Line, uint8_t Foreground, uint8_t Background)
{
    uint8_t Character = 0;
    while (*str)
    {
        outputcharp(*str, Character, Line, Foreground, Background);
        Character++;
        str++;
    }
}

void printString(const char* str, uint8_t Foreground, uint8_t Background)
{
    while (*str)
    {
        outputchar(*str, Foreground, Background);
        str++;
    }
}

void initScreen()
{
    clearscreen();
}


void outputcharp(char character, uint8_t Row, uint8_t Line, uint8_t Foreground, uint8_t Background)
{
    uint8_t color = Background << 4;
    color |= Foreground;
    putcharp(Row, Line, character, (Background << 4) | Foreground);
}

void outputchar(char character, uint8_t Foreground, uint8_t Background)
{
    uint8_t color = Background << 4;
    color |= Foreground;
    putchar(character, (Background << 4) | Foreground);
}

void removeLastCharacter()
{
    uint8_t XPos = getXPos();
    uint8_t YPos = getYPos();
    setcursor(XPos - 1, YPos);
    uint8_t lastcolor = getColor(XPos - 1, YPos);
    putchar(' ', lastcolor);
    setcursor(XPos - 1, YPos);
}

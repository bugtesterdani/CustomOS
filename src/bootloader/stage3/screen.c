#include "headers/stdint.h"
#include "headers/screen.h"
#include "headers/io.h"

uint8_t* g_ScreenBuffer = (uint8_t*)SCREEN_ADDR;
uint8_t ScreenX = 0;
uint8_t ScreenY = 0;

void putcharp(int x, int y, char character, uint8_t color)
{
    g_ScreenBuffer[2 * (y * SCREEN_WIDTH + x)] = character;
    g_ScreenBuffer[2 * (y * SCREEN_WIDTH + x) + 1] = color;
}

void putchar(char character, uint8_t color)
{
    g_ScreenBuffer[2 * (ScreenY * SCREEN_WIDTH + ScreenX)] = character;
    g_ScreenBuffer[2 * (ScreenY * SCREEN_WIDTH + ScreenX) + 1] = color;
    ScreenX++;
    if (ScreenX == 80)
    {
        ScreenX = 0;
        ScreenY++;
    }
    updatePos();
}

char getchar(int x, int y)
{
    return g_ScreenBuffer[2 * (y * SCREEN_WIDTH + x)];
}

uint8_t getXPos()
{
    return ScreenX;
}

uint8_t getYPos()
{
    return ScreenY;
}

uint8_t getColor(int x, int y)
{
    return g_ScreenBuffer[2 * (y * SCREEN_WIDTH + x) + 1];
}

void setcursor(int x, int y)
{
    ScreenX = x;
    ScreenY = y;
    updatePos();
}

void setcursorX(int x)
{
    ScreenX = x;
    updatePos();
}

void setcursornewline()
{
    ScreenX = 0;
    ScreenY++;
    if (ScreenY == 25)
    {
        scrollDown();
    }
    updatePos();
}

void updatePos()
{
    unsigned short    position=(ScreenY*80) + ScreenX;
    // cursor LOW port to vga INDEX register
    outportb(0x3D4, 0x0F);
    outportb(0x3D5, (unsigned char)(position&0xFF));
    // cursor HIGH port to vga INDEX register
    outportb(0x3D4, 0x0E);
    outportb(0x3D5, (unsigned char)((position>>8)&0xFF));
}

void clearscreen()
{
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            putcharp(x, y, '\0', DEFAULT_COLOR);
        }
    }
    setcursor(0, 0);
}

void scrollDown()
{
    for (int y = 1; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            putcharp(x, y - 1, getchar(x, y), getColor(x, y));
        }
    }
    for (int y = SCREEN_HEIGHT - 1; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            putcharp(x, y, '\0', DEFAULT_COLOR);
        }
    }
    ScreenY -= 1;
}

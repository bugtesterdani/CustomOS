#pragma once

#include "stdint.h"
#include "io.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define DEFAULT_COLOR 0x00

#define SCREEN_ADDR 0xB0000000

void screen_setup_static(uint32_t address_X, uint32_t address_Y);
void putcharp(int x, int y, char character, uint8_t color);
void putchar(char character, uint8_t color);
void setcursor(int x, int y);
void setcursorX(int x);
void setcursornewline();
void clearscreen();
void scrollDown();
void updatePos();
uint8_t getXPos();
uint8_t getYPos();
uint8_t getColor(int x, int y);
void printString(char *str, uint8_t Foreground, uint8_t Background);

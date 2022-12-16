#pragma once

#include "stdint.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define DEFAULT_COLOR 0xF0

#define SCREEN_ADDR 0xB8000

void putcharp(int x, int y, char character, uint8_t color);
void putchar(char character, uint8_t color);
void setcursor(int x, int y);
void clearscreen();
void scrollDown();
void updatePos();
uint8_t getXPos();
uint8_t getYPos();
uint8_t getColor(int x, int y);

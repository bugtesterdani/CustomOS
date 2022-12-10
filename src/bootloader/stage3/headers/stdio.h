#pragma once

#include "stdint.h"

void print(const char* str, uint8_t Line, uint8_t Foreground, uint8_t Background);
void initScreen();
void outputchar(char character, uint8_t Row, uint8_t Line, uint8_t Foreground, uint8_t Background);

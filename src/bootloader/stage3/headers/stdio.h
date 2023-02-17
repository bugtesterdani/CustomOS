#pragma once

#include "stdint.h"

void print(const char* str, uint8_t Line, uint8_t Foreground, uint8_t Background);
void printString(const char* str, uint8_t Foreground, uint8_t Background);
void initScreen();
void outputcharp(char character, uint8_t Row, uint8_t Line, uint8_t Foreground, uint8_t Background);
void outputchar(char character, uint8_t Foreground, uint8_t Background);
void removeLastCharacter();
void clearArray(uint8_t* Array, unsigned int length, char default_value);

#include "headers/RAM.h"

#include "headers/screen.h"
#include "headers/commands.h"
#include "headers/fat32.h"
#include "headers/string.h"
#include "headers/colors.h"
#include "headers/stdio.h"

void RAM_Init()
{
    uint8_t* value = (uint8_t*)0x7E00;
    uint8_t CHARARRAYMAX = 80;
    uint8_t outputint[CHARARRAYMAX];
    uint8_t _lastindex;
    clearArray(outputint, CHARARRAYMAX, 0x00);
    outputint[0] = '0';
    outputint[1] = 'x';
    ConvertToChar(value[0], 16, outputint, 2);
    _lastindex = lastIndex(outputint, CHARARRAYMAX);
    outputint[_lastindex] = ' ';
    ConvertToChar(value[1], 16, outputint, _lastindex + 1);
    _lastindex = lastIndex(outputint, CHARARRAYMAX);
    outputint[_lastindex] = ' ';
    ConvertToChar(value[2], 16, outputint, _lastindex + 1);
    _lastindex = lastIndex(outputint, CHARARRAYMAX);
    outputint[_lastindex] = ' ';
    ConvertToChar(value[3], 16, outputint, _lastindex + 1);
    _lastindex = lastIndex(outputint, CHARARRAYMAX);
    outputint[_lastindex] = ' ';
    ConvertToChar(value[4], 16, outputint, _lastindex + 1);
    _lastindex = lastIndex(outputint, CHARARRAYMAX);
    outputint[_lastindex] = ' ';
    ConvertToChar(value[5], 16, outputint, _lastindex + 1);
    _lastindex = lastIndex(outputint, CHARARRAYMAX);
    outputint[_lastindex] = ' ';
    ConvertToChar(value[6], 16, outputint, _lastindex + 1);
    _lastindex = lastIndex(outputint, CHARARRAYMAX);
    outputint[_lastindex] = ' ';
    ConvertToChar(value[7], 16, outputint, _lastindex + 1);
    printString(outputint, White, Black);
    setcursornewline();
}
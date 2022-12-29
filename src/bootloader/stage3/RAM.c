#include "headers/RAM.h"

#include "headers/screen.h"
#include "headers/commands.h"
#include "headers/fat32.h"
#include "headers/string.h"
#include "headers/colors.h"
#include "headers/stdio.h"

void RAM_Init()
{
    raminit_1();
    uint8_t* values = (uint8_t*)0x7E00;
    uint8_t size = values[0];
    RAM_t* RAM_Values = (RAM_t*)0x7E01;
    for (uint8_t i = 0; i < size; i++)
    {
        RAM_t RAM = RAM_Values[i];
        ram_subPrint(64, RAM.BaseAddress);
        ram_subPrint(64, RAM.Size);
        ram_subPrint(32, RAM.Type);
        setcursornewline();
    }
}

void raminit_1()
{
    uint8_t* value = (uint8_t*)0x7E00;
    uint8_t CHARARRAYMAX = 80;
    uint8_t outputint[CHARARRAYMAX];
    uint8_t _lastindex;
    uint8_t maxindex;
    clearArray(outputint, CHARARRAYMAX, 0x00);
    ConvertToChar(value[0], 16, outputint, 0);
    printString(outputint, White, Black);
    setcursornewline();
    for (uint8_t k = 0; k < value[0]; k++)
    {
        for (uint8_t i = 0; i < 3; i++)
        {
            if (i < 2)
            {
                clearArray(outputint, CHARARRAYMAX, 0x00);
                outputint[0] = '0';
                outputint[1] = 'x';
                for (uint8_t j = (i * 8); j < (i + 1) * 8; j++)
                {
                    _lastindex = lastIndex(outputint, CHARARRAYMAX);

                    uint8_t tmp_char[5];
                    clearArray(tmp_char, 5, 0x00);
                    ConvertToChar(value[j + (k * 3 * 8) + 1], 16, tmp_char, 0);
                    FillStrSize(tmp_char, '0', 2);

                    strapp(outputint, tmp_char, _lastindex, CHARARRAYMAX);
                }
                _lastindex = lastIndex(outputint, CHARARRAYMAX);
                outputint[_lastindex] = ' ';
            }
            else
            {
                clearArray(outputint, CHARARRAYMAX, 0x00);
                outputint[0] = '0';
                outputint[1] = 'x';
                for (uint8_t j = (i * 8); j < (i * 8) + 4; j++)
                {
                    _lastindex = lastIndex(outputint, CHARARRAYMAX);

                    uint8_t tmp_char[5];
                    clearArray(tmp_char, 5, 0x00);
                    ConvertToChar(value[j + ((k * 2 * 8) + (k * 4)) + 1], 16, tmp_char, 0);
                    FillStrSize(tmp_char, '0', 2);

                    strapp(outputint, tmp_char, _lastindex, CHARARRAYMAX);
                }
            }
            printString(outputint, White, Black);
        }
        setcursornewline();
    }
}

void ram_parseToCharArr(char *str, uint16_t value, uint8_t charmax)
{
    uint8_t tmp_char[5];
    uint8_t _lastindex = lastIndex(str, charmax);
    clearArray(tmp_char, 5, 0x00);
    ConvertToChar(value, 16, tmp_char, 0);
    FillStrSize(tmp_char, '0', 4);
    strapp(str, tmp_char, _lastindex, charmax);
}

void ram_subPrint(uint8_t size, uint64_t value)
{
    uint8_t CHARARRAYMAX = 80;
    uint8_t outputint[CHARARRAYMAX];
    uint8_t _lastindex;

    clearArray(outputint, CHARARRAYMAX, 0x00);
    outputint[0] = '0';
    outputint[1] = 'x';
    for (uint8_t i = size; i >= 16; i -= 16)
    {
        ram_parseToCharArr(outputint, ((value >> (i - 16)) & 0xFFFF), CHARARRAYMAX);
    }
    _lastindex = lastIndex(outputint, CHARARRAYMAX);
    outputint[_lastindex] = ' ';
    printString(outputint, White, Black);
}

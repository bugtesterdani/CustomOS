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
    // RAM_t* RAM_Values = (RAM_t*)0x7E01;
    // RAM_t RValues[size];
    // uint8_t counter = 1;
    // for (int i = 0; i < size + 1; i++)
    // {
    //     RValues[i].BaseAddress =
    //         (((values[counter +  0] & 0xFF) << 56) |
    //          ((values[counter +  1] & 0xFF) << 48) |
    //          ((values[counter +  2] & 0xFF) << 40) |
    //          ((values[counter +  3] & 0xFF) << 32) |
    //          ((values[counter +  4] & 0xFF) << 24) |
    //          ((values[counter +  5] & 0xFF) << 16) |
    //          ((values[counter +  6] & 0xFF) <<  8) |
    //          ((values[counter +  7] & 0xFF) <<  0));
    //     RValues[i].Size =
    //         (((values[counter +  8] & 0xFF) << 56) |
    //          ((values[counter +  9] & 0xFF) << 48) |
    //          ((values[counter + 10] & 0xFF) << 40) |
    //          ((values[counter + 11] & 0xFF) << 32) |
    //          ((values[counter + 12] & 0xFF) << 24) |
    //          ((values[counter + 13] & 0xFF) << 16) |
    //          ((values[counter + 14] & 0xFF) <<  8) |
    //          ((values[counter + 15] & 0xFF) <<  0));
    //     RValues[i].Type = 
    //         (((values[counter + 16] & 0xFF) << 24) |
    //          ((values[counter + 17] & 0xFF) << 16) |
    //          ((values[counter + 18] & 0xFF) <<  8) |
    //          ((values[counter + 19] & 0xFF) <<  0));
    //     counter += 20;
    // }
    RAM_t RValues[size];
    for (int i = 0; i < size; i++) {   // 6 Einträge in der Map
        // Lese Eintrag aus Speicher
        RValues[i].BaseAddress = *(uint64_t*)(0x7E04 + i * 0x18);
        RValues[i].Size = *(uint64_t*)(0x7E0C + i * 0x18);
        RValues[i].Type = *(uint32_t*)(0x7E14 + i * 0x18);
        RValues[i].ExtendedPart = *(uint32_t*)(0x7E18 + i * 0x18);
        // Verarbeite Eintrag ...
    }

    for (uint8_t i = 0; i < size; i++)
    {
        // RAM_t RAM = RAM_Values[i];
        RAM_t RAM = RValues[i];
        ram_subPrint(64, RAM.BaseAddress);
        ram_subPrint(64, RAM.Size);
        ram_subPrint(32, RAM.Type);
        ram_subPrint(32, RAM.ExtendedPart);
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
    uint8_t counter = 1;
    for (uint8_t k = 0; k < value[0] + 1; k++)
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
                    ConvertToChar(value[counter], 16, tmp_char, 0);
                    counter++;
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
                for (uint8_t j = (i * 8); j < (i * 8) + 8; j++)
                {
                    _lastindex = lastIndex(outputint, CHARARRAYMAX);

                    uint8_t tmp_char[5];
                    clearArray(tmp_char, 5, 0x00);
                    ConvertToChar(value[counter], 16, tmp_char, 0);
                    counter++;
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
    FillStrSize(tmp_char, '0', 2);
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
    for (uint8_t i = size; i >= 8; i -= 8)
    {
        ram_parseToCharArr(outputint, ((value >> (i - 8)) & 0xFF), CHARARRAYMAX);
    }
    _lastindex = lastIndex(outputint, CHARARRAYMAX);
    outputint[_lastindex] = ' ';
    printString(outputint, White, Black);
}

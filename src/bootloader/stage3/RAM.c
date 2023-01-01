#include "headers/RAM.h"

#include "headers/screen.h"
#include "headers/commands.h"
#include "headers/fat32.h"
#include "headers/string.h"
#include "headers/colors.h"
#include "headers/stdio.h"

void RAM_Init()
{
    uint8_t* values = (uint8_t*)RAM_OFFSET;
    uint8_t size = values[0];
    RAM_t RValues[size];
    for (int i = 0; i < size; i++) {   // 6 Einträge in der Map
        // Lese Eintrag aus Speicher
        RValues[i].BaseAddress = *(uint64_t*)(RAM_OFFSET + 0x04 + i * 0x18);
        RValues[i].Size = *(uint64_t*)(RAM_OFFSET + 0x0C + i * 0x18);
        RValues[i].Type = *(uint32_t*)(RAM_OFFSET + 0x14 + i * 0x18);
        RValues[i].ExtendedPart = *(uint32_t*)(RAM_OFFSET + 0x18 + i * 0x18);
        // Verarbeite Eintrag ...
    }

    uint64_t sizeBytes_usedKernel = 0;
    uint64_t sizeBytes_Available = 0;
    uint64_t sizeBytes_Total = 0;

    for (uint8_t i = 0; i < size; i++)
    {
        // RAM_t RAM = RAM_Values[i];
        RAM_t RAM = RValues[i];
        ram_subPrint(64, RAM.BaseAddress);
        ram_subPrint(64, RAM.Size);
        ram_subPrint(32, RAM.Type);
        ram_subPrint(32, RAM.ExtendedPart);
        setcursornewline();
        uint8_t tmp_char[80];
        switch (RAM.Type)
        {
            case RAM_Type_Available:
                clearArray(tmp_char, 80, 0x00);
                strapp(tmp_char, "RAM Verfuegbar", 0, 80);
                printString(tmp_char, White, Black);
                setcursornewline();
                if (RAM.BaseAddress == 0x00)
                {
                    sizeBytes_usedKernel = RAM.Size;
                }
                else
                {
                    sizeBytes_Available += RAM.Size;
                }
                sizeBytes_Total += RAM.Size;
                break;
            
            case RAM_Type_Reserved:
                clearArray(tmp_char, 80, 0x00);
                strapp(tmp_char, "RAM Reserviert", 0, 80);
                printString(tmp_char, White, Black);
                setcursornewline();
                sizeBytes_Total += RAM.Size;
                break;

            case RAM_Type_ACPI:
                clearArray(tmp_char, 80, 0x00);
                strapp(tmp_char, "ACPI Reserviert", 0, 80);
                printString(tmp_char, White, Black);
                setcursornewline();
                sizeBytes_Total += RAM.Size;
                break;

            case RAM_Type_Error:
                clearArray(tmp_char, 80, 0x00);
                strapp(tmp_char, "RAM Space marked with errors", 0, 80);
                printString(tmp_char, White, Black);
                setcursornewline();
                sizeBytes_Total += RAM.Size;
                break;

            default:
                clearArray(tmp_char, 80, 0x00);
                strapp(tmp_char, "Unknown Error RAM", 0, 80);
                printString(tmp_char, White, Black);
                setcursornewline();
                sizeBytes_Total += RAM.Size;
                break;
        }
    }
    ram_subPrint(64, sizeBytes_Available);
    ram_printSize("Available:     ", sizeBytes_Available);
    ram_subPrint(64, sizeBytes_usedKernel);
    ram_printSize("Used by Kernel:", sizeBytes_usedKernel);
    ram_subPrint(64, sizeBytes_Total);
    ram_printSize("Total:         ", sizeBytes_Total);
}

void ram_printSize(char *str, uint64_t size)
{
    uint64_t sizekb = size   / 1000;    // kilobit
    size -= (sizekb * 1000);
    uint64_t sizeMb = sizekb / 1000;    // megabit
    sizekb -= (sizeMb * 1000);
    uint64_t sizegb = sizeMb / 1000;    // gigabit
    sizeMb -= (sizegb * 1000);

    uint8_t output[80];
    uint8_t _lastindex;
    clearArray(output, 80, 0x00);
    strapp(output, str, 0, 80);
    if (sizegb != 0)
    {
        _lastindex = lastIndex(output, 80);
        if (_lastindex != 0)
        {
            output[_lastindex] = ' ';
            _lastindex++;
        }
        ConvertToChar(sizegb, 10, output, _lastindex);
        _lastindex = lastIndex(output, 80);
        strapp(output, "Gb", _lastindex, 80);
    }
    if (sizeMb != 0)
    {
        _lastindex = lastIndex(output, 80);
        if (_lastindex != 0)
        {
            output[_lastindex] = ' ';
            _lastindex++;
        }
        ConvertToChar(sizeMb, 10, output, _lastindex);
        _lastindex = lastIndex(output, 80);
        strapp(output, "Mb", _lastindex, 80);
    }
    if (sizekb != 0)
    {
        _lastindex = lastIndex(output, 80);
        if (_lastindex != 0)
        {
            output[_lastindex] = ' ';
            _lastindex++;
        }
        ConvertToChar(sizekb, 10, output, _lastindex);
        _lastindex = lastIndex(output, 80);
        strapp(output, "kb", _lastindex, 80);
    }
    if (size != 0)
    {
        _lastindex = lastIndex(output, 80);
        if (_lastindex != 0)
        {
            output[_lastindex] = ' ';
            _lastindex++;
        }
        ConvertToChar(size, 10, output, _lastindex);
        _lastindex = lastIndex(output, 80);
        strapp(output, "b", _lastindex, 80);
    }
    printString(output, White, Black);
    setcursornewline();
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

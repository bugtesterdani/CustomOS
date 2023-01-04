#include "headers/RAM.h"
#include "headers/memory_management.h"

#include "headers/screen.h"
#include "headers/commands.h"
#include "headers/fat32.h"
#include "headers/string.h"
#include "headers/colors.h"
#include "headers/stdio.h"

void RAM_FullInit()
{
    RAM_t* RAM;
    uint8_t* values = (uint8_t*)RAM_OFFSET;
    uint8_t size_RAM = values[0];

    for (int i = 0; i < values[0]; i++)
    {
        // Lese Eintrag aus Speicher
        RAM[i].BaseAddress = *(uint64_t*)(RAM_OFFSET + 0x04 + i * 0x18);
        RAM[i].Size = *(uint64_t*)(RAM_OFFSET + 0x0C + i * 0x18);
        RAM[i].Type = *(uint32_t*)(RAM_OFFSET + 0x14 + i * 0x18);
        RAM[i].ExtendedPart = *(uint32_t*)(RAM_OFFSET + 0x18 + i * 0x18);
        // Verarbeite Eintrag ...
    }

    uint32_t* _memory_pointer = (uint32_t*)0x500;
    for (uint64_t i = 0; i < ((0x900 - 0x500) / (32 / 8)); i++)
    {
        _memory_pointer[i] = (uint32_t)0x00;
    }

    uint8_t output[80];
    uint8_t _lastindex;
    for (uint16_t i = 0; i < size_RAM; i++)
    {
        clearArray(output, 80, 0x00);
        output[0] = ' ';
        ConvertToChar(((RAM[i].BaseAddress >> 48) & 0xFFFF), 16, output, 1);
        _lastindex = lastIndex(output, 80);
        ConvertToChar(((RAM[i].BaseAddress >> 32) & 0xFFFF), 16, output, _lastindex);
        _lastindex = lastIndex(output, 80);
        ConvertToChar(((RAM[i].BaseAddress >> 16) & 0xFFFF), 16, output, _lastindex);
        _lastindex = lastIndex(output, 80);
        ConvertToChar(((RAM[i].BaseAddress >>  0) & 0xFFFF), 16, output, _lastindex);
        _lastindex = lastIndex(output, 80);
        output[_lastindex] = ' ';
        ConvertToChar(((RAM[i].Size >> 48) & 0xFFFF), 16, output, _lastindex);
        _lastindex = lastIndex(output, 80);
        ConvertToChar(((RAM[i].Size >> 32) & 0xFFFF), 16, output, _lastindex);
        _lastindex = lastIndex(output, 80);
        ConvertToChar(((RAM[i].Size >> 16) & 0xFFFF), 16, output, _lastindex);
        _lastindex = lastIndex(output, 80);
        ConvertToChar(((RAM[i].Size >>  0) & 0xFFFF), 16, output, _lastindex);
        printString(output, White, Black);
        setcursornewline();
    }

    uint64_t sizeBytes_usedKernel = 0;
    uint64_t sizeBytes_Available = 0;
    uint64_t sizeBytes_Total = 0;

    for (int i = 0; i < size_RAM; i++)
    {
        // RAM_t RAM = RAM_Values[i];
        RAM_t RAM_Value = RAM[i];
        uint8_t tmp_char[80];
        switch (RAM_Value.Type)
        {
            case RAM_Type_Available:
                if (RAM_Value.BaseAddress == 0x00)
                {
                    sizeBytes_usedKernel = RAM_Value.Size;
                }
                else
                {
                    register_memspace(&RAM_Value);
                    sizeBytes_Available += RAM_Value.Size;
                }
                sizeBytes_Total += RAM_Value.Size;
                break;
            
            case RAM_Type_Reserved:
                sizeBytes_Total += RAM_Value.Size;
                break;

            case RAM_Type_ACPI:
                sizeBytes_Total += RAM_Value.Size;
                break;

            case RAM_Type_Error:
                sizeBytes_Total += RAM_Value.Size;
                break;

            default:
                sizeBytes_Total += RAM_Value.Size;
                break;
        }
    }
    ram_printSize("Available:     ", sizeBytes_Available);
    ram_printSize("Used by Kernel:", sizeBytes_usedKernel);
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

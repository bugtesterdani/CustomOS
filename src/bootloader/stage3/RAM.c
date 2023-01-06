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
    uint8_t* values = (uint8_t*)RAM_OFFSET;
    uint8_t size_RAM = values[0];
    RAM_t* RAM = (RAM_t*)(RAM_OFFSET + 0x04 + (size_RAM * 0x18));

    uint8_t output[80];
    uint8_t _lastindex;
    for (int i = 0; i < values[0]; i++)
    {
        // Lese Eintrag aus Speicher
        RAM[i].BaseAddress = *(uint64_t*)(RAM_OFFSET + 0x04 + i * 0x18);
        RAM[i].Size = *(uint64_t*)(RAM_OFFSET + 0x0C + i * 0x18);
        RAM[i].Type = *(uint32_t*)(RAM_OFFSET + 0x14 + i * 0x18);
        RAM[i].ExtendedPart = *(uint32_t*)(RAM_OFFSET + 0x18 + i * 0x18);
        // Verarbeite Eintrag ...
        clearArray(output, 80, 0x00);
        uint8_t baseaddr[20];
        clearArray(baseaddr, 20, 0x00);
        ConvertToChar(((RAM[i].BaseAddress) >> 48) & 0xFFFF, 16, baseaddr, 0);
        FillStrSize(baseaddr, '0', 4);
        strapp(output, baseaddr, 0, 20);
        _lastindex = lastIndex(output, 80);
        clearArray(baseaddr, 20, 0x00);
        ConvertToChar(((RAM[i].BaseAddress) >> 32) & 0xFFFF, 16, baseaddr, 0);
        FillStrSize(baseaddr, '0', 4);
        strapp(output, baseaddr, _lastindex, 20);
        _lastindex = lastIndex(output, 80);
        clearArray(baseaddr, 20, 0x00);
        ConvertToChar(((RAM[i].BaseAddress) >> 16) & 0xFFFF, 16, baseaddr, 0);
        FillStrSize(baseaddr, '0', 4);
        strapp(output, baseaddr, _lastindex, 20);
        _lastindex = lastIndex(output, 80);
        clearArray(baseaddr, 20, 0x00);
        ConvertToChar(((RAM[i].BaseAddress) >>  0) & 0xFFFF, 16, baseaddr, 0);
        FillStrSize(baseaddr, '0', 4);
        strapp(output, baseaddr, _lastindex, 20);
        _lastindex = lastIndex(output, 80);
        output[_lastindex] = ' ';
        clearArray(baseaddr, 20, 0x00);
        ConvertToChar(((RAM[i].Size) >> 48) & 0xFFFF, 16, baseaddr, 0);
        FillStrSize(baseaddr, '0', 4);
        strapp(output, baseaddr, _lastindex + 1, 20);
        _lastindex = lastIndex(output, 80);
        clearArray(baseaddr, 20, 0x00);
        ConvertToChar(((RAM[i].Size) >> 32) & 0xFFFF, 16, baseaddr, 0);
        FillStrSize(baseaddr, '0', 4);
        strapp(output, baseaddr, _lastindex, 20);
        _lastindex = lastIndex(output, 80);
        clearArray(baseaddr, 20, 0x00);
        ConvertToChar(((RAM[i].Size) >> 16) & 0xFFFF, 16, baseaddr, 0);
        FillStrSize(baseaddr, '0', 4);
        strapp(output, baseaddr, _lastindex, 20);
        _lastindex = lastIndex(output, 80);
        clearArray(baseaddr, 20, 0x00);
        ConvertToChar(((RAM[i].Size) >>  0) & 0xFFFF, 16, baseaddr, 0);
        FillStrSize(baseaddr, '0', 4);
        strapp(output, baseaddr, _lastindex, 20);
        _lastindex = lastIndex(output, 80);
        output[_lastindex] = ' ';
        clearArray(baseaddr, 20, 0x00);
        ConvertToChar(((RAM[i].Type) >> 16) & 0xFFFF, 16, baseaddr, 0);
        FillStrSize(baseaddr, '0', 4);
        strapp(output, baseaddr, _lastindex + 1, 20);
        _lastindex = lastIndex(output, 80);
        clearArray(baseaddr, 20, 0x00);
        ConvertToChar(((RAM[i].Type) >>  0) & 0xFFFF, 16, baseaddr, 0);
        FillStrSize(baseaddr, '0', 4);
        strapp(output, baseaddr, _lastindex, 20);
        _lastindex = lastIndex(output, 80);
        output[_lastindex] = ' ';
        clearArray(baseaddr, 20, 0x00);
        ConvertToChar(((RAM[i].ExtendedPart) >> 16) & 0xFFFF, 16, baseaddr, 0);
        FillStrSize(baseaddr, '0', 4);
        strapp(output, baseaddr, _lastindex + 1, 20);
        _lastindex = lastIndex(output, 80);
        clearArray(baseaddr, 20, 0x00);
        ConvertToChar(((RAM[i].ExtendedPart) >>  0) & 0xFFFF, 16, baseaddr, 0);
        FillStrSize(baseaddr, '0', 4);
        strapp(output, baseaddr, _lastindex, 20);
        _lastindex = lastIndex(output, 80);
        output[_lastindex] = ' ';
        printString(output, White, Black);
        setcursornewline();
    }

    uint32_t* _memory_pointer = (uint32_t*)RAM_OFFSET;
    for (uint32_t i = 0; i < ((values[0] * 0x06) + 0x01); i++)
    {
        _memory_pointer[i] = (uint32_t)0x00;
    }

    uint64_t sizeBytes_usedKernel = 0;
    uint64_t sizeBytes_Available = 0;
    uint64_t sizeBytes_Total = 0;

    for (int i = 0; i < size_RAM; i++)
    {
        // RAM_t RAM = RAM_Values[i];
        RAM_t RAM_Value = RAM[i];
        for (uint8_t j = 0; j < 0x06; j++)
        {
            _memory_pointer[((values[0] * 0x06) + 0x01) + j] = (uint32_t)0x00;
        }
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

    for (uint32_t i = 0; i < (((0x900 - RAM_OFFSET) / (32 / 8)) - (((values[0] * 0x06) + 0x01) * 2)); i++)
    {
        _memory_pointer[((values[0] * 0x06) + 0x01) + i] = (uint32_t)0x00;
    }

    register_memory_map();

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

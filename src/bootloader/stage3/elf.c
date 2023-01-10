#include "headers/elf.h"


#include "headers/colors.h"
#include "headers/screen.h"
#include "headers/string.h"
#include "headers/stdio.h"
#include "headers/commands.h"
#include "headers/fat32.h"

uint8_t parseELFFile(uint16_t *elffile, uint32_t sizeBytes)
{
    // Check if its an ELF File
    if (elffile[0] != 0x7F45 &&
        elffile[1] != 0x4c46)
    {
        char output[80];
        clearArray(output, 80, 0x00);
        output[0] = ' ';
        ConvertToChar(elffile[0], 16, output, 1);
        uint8_t _lastindex = lastIndex(output, 80);
        ConvertToChar(elffile[1], 16, output, _lastindex);
        printString(output, White, Black);
        printString(" Error Parsing 1", White, Black);
        return 0;
    }

    // Check if its an executable file
    if (elffile[5] != 0x0200)
    {
        char output[80];
        clearArray(output, 80, 0x00);
        ConvertToChar(elffile[0], 16, output, 0);
        printString(output, White, Black);
        printString(" Error Parsing 2", White, Black);
        return 0;
    }

    printString("Successfully parsed ELF file", White, Black);
    return 1;
}

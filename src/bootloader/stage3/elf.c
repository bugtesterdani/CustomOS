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
    if (((uint8_t*)elffile)[0] != 0x7F &&
        ((uint8_t*)elffile)[1] != 0x45 &&
        ((uint8_t*)elffile)[2] != 0x4c &&
        ((uint8_t*)elffile)[3] != 0x46)
    {
        printString("Error Parsing 1", White, Black);
        return 0;
    }

    // Check if its an executable file
    if (((uint8_t*)elffile)[10] != 0x02 &&
        ((uint8_t*)elffile)[11] != 0x00)
    {
        printString("Error Parsing 2", White, Black);
        return 0;
    }

    printString("Successfully parsed ELF file", White, Black);
    return 1;
}

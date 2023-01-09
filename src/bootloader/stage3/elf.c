#include "headers/elf.h"


#include "headers/colors.h"
#include "headers/screen.h"
#include "headers/string.h"
#include "headers/stdio.h"
#include "headers/commands.h"

uint8_t parseELFFile(uint8_t *elffile, uint64_t size)
{
    // Check if its an ELF File
    if (elffile[0] != 0x7F &&
        elffile[1] != 0x45 &&
        elffile[2] != 0x4c &&
        elffile[3] != 0x46)
    {
        return 0;
    }

    // Check if its an executable file
    if (elffile[10] != 0x02 &&
        elffile[11] != 0x00)
    {
        return 0;
    }

    uint8_t output[80];
    printString(output, White, Black);
    return 1;
}

#include "headers/commands.h"
#include "headers/colors.h"
#include "headers/stdio.h"
#include "headers/string.h"
#include "headers/stdint.h"
#include "headers/fat32.h"

void ParseCommand(char* commandline)
{
    char PartSplit[60];
    clearArray(PartSplit, 60, 0x00);
    int spos = 0;
    
    // Get First Argument
    // SplitParameters(commandline, &spos, PartSplit);
    // printString(PartSplit, White, Black);
    
    // Get Second Argument
    // clearArray(PartSplit, 60, 0x00);
    // spos++;
    // SplitParameters(commandline, &spos, PartSplit);
    // printString(PartSplit, White, Black);

    SplitParameters(commandline, &spos, PartSplit);
    if (strcmp(PartSplit, "help"))
    {
        printString("Help Message. Type help for this Message.", White, Black);
    }
    else if (strcmp(PartSplit, "ping"))
    {
        printString("pong", White, Black);
    }
    else if (strcmp(PartSplit, "oem"))
    {
        FAT32_t *fat32;
        ReadParameter(fat32);
    }
    else if (strcmp(PartSplit, "hexing"))
    {
        clearArray(PartSplit, 60, 0x00);
        spos++;
        SplitParameters(commandline, &spos, PartSplit);
        char outputint[20];
        ConvertHexToChar(PartSplit[0], outputint);
        printString(outputint, White, Black);
    }
    else
    {
        printString("Command not now implemented please again.", White, Black);
    }
}

void SplitParameters(char* line, int *start_pos, char* PartSplit)
{
    int counter = *start_pos;
    while (counter > 0)
    {
        line++;
        counter--;
    }
    while (*line && *line != ' ')
    {
        PartSplit[counter] = *line;
        counter++;
        line++;
    }
    *start_pos = counter;
    PartSplit[counter] = '\0';
}

void clearArray(char* Array, unsigned int length, char default_value)
{
    for (unsigned int i = 0; i < length; i++)
    {
        Array[i] = default_value;
    }
}

#include "headers/commands.h"
#include "headers/colors.h"
#include "headers/stdio.h"
#include "headers/crc32.h"

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
    int value = crc32_tabledriven(PartSplit, 60);
    char Output[15];
    clearArray(Output, 15, 0x00);
    ConvertIntToChar(value, Output);
    printString(Output, White, Black);
}

void ConvertIntToChar(int value, char* Output)
{
    for (uint8_t i = 10; i > 0; i--)
    {
        uint8_t tmp_value = Number(value, power(10, i));
        value -= power(10, i) * tmp_value;
        Output[10 - i] = NumberToChar(tmp_value);
    }
}

int power(int base, unsigned int exp)
{
    int result = 1;
    for (int i = 0; i < exp; i++)
        result *= base;
    return result;
}

char NumberToChar(int value)
{
    if (value >= 9)
    {
        return '9';
    }
    if (value >= 8)
    {
        return '8';
    }
    if (value >= 7)
    {
        return '7';
    }
    if (value >= 6)
    {
        return '6';
    }
    if (value >= 5)
    {
        return '5';
    }
    if (value >= 4)
    {
        return '4';
    }
    if (value >= 3)
    {
        return '3';
    }
    if (value >= 2)
    {
        return '2';
    }
    if (value >= 1)
    {
        return '1';
    }
    return '0';
}

unsigned int Number(int value, int multiplicator)
{
    if (value >= (9 * multiplicator))
    {
        return 9;
    }
    if (value >= (8 * multiplicator))
    {
        return 8;
    }
    if (value >= (7 * multiplicator))
    {
        return 7;
    }
    if (value >= (6 * multiplicator))
    {
        return 6;
    }
    if (value >= (5 * multiplicator))
    {
        return 5;
    }
    if (value >= (4 * multiplicator))
    {
        return 4;
    }
    if (value >= (3 * multiplicator))
    {
        return 3;
    }
    if (value >= (2 * multiplicator))
    {
        return 2;
    }
    if (value >= (1 * multiplicator))
    {
        return 1;
    }
    return 0;
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

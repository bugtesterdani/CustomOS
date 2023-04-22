#include "headers/stdint.h"

char NumberToChar(int value)
{
    if (value >= 15)
    {
        return 'F';
    }
    if (value >= 14)
    {
        return 'E';
    }
    if (value >= 13)
    {
        return 'D';
    }
    if (value >= 12)
    {
        return 'C';
    }
    if (value >= 11)
    {
        return 'B';
    }
    if (value >= 10)
    {
        return 'A';
    }
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

void ConvertToChar(uint16_t value, int base, char* Output, int startindex)
{
    int i = startindex;
    if (value <= 0)
    {
        Output[i] = '0';
        i++;
    }

    while (value > 0)
    {
        Output[i] = NumberToChar(value % base);
        value -= (value % base);
        value /= base;
        i++;
    }
    Output[i] = '\0';
    for (int j = 0; j < (i - startindex) / 2; j++)
    {
        char tmp = Output[j + startindex];
        Output[j + startindex] = Output[i - j - 1];
        Output[i - j - 1] = tmp;
    }
}

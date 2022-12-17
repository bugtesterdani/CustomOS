#include "headers/stdint.h"

void ConvertIntToChar(int value, char* Output)
{
    for (uint8_t i = 10; i > 0; i--)
    {
        uint8_t tmp_value = Number(value, power(10, i));
        value -= power(10, i) * tmp_value;
        Output[10 - i] = NumberToChar(tmp_value);
    }
}

void ConvertHexToChar(int value, char* Output)
{
    for (uint8_t i = 10; i > 0; i--)
    {
        uint8_t tmp_value = NumberHEX(value, power(16, i));
        value -= power(16, i) * tmp_value;
        Output[10 - i] = NumberHEXToChar(tmp_value);
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

char NumberHEXToChar(int value)
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
    return NumberToChar(value);
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

unsigned int NumberHEX(int value, int multiplicator)
{
    if (value >= (15 * multiplicator))
    {
        return 15;
    }
    if (value >= (14 * multiplicator))
    {
        return 14;
    }
    if (value >= (13 * multiplicator))
    {
        return 13;
    }
    if (value >= (12 * multiplicator))
    {
        return 12;
    }
    if (value >= (11 * multiplicator))
    {
        return 11;
    }
    if (value >= (10 * multiplicator))
    {
        return 10;
    }
    return Number(value, multiplicator);
}

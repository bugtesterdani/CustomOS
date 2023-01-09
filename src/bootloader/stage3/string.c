#include "headers/string.h"

bool strcmp(char *str1, char *str2)
{
    while (*str1 || *str2)
    {
        if (*str1 != *str2)
        {
            return false;
        }
        str1++;
        str2++;
    }
    return true;
}

uint8_t cmplsname(char *str1, char *str2)
{
    bool found_dot = false;
    while (*str1 || *str2)
    {
        if ((*str1 == ' ') &&
            (*str2 == '.'))
        {
            found_dot = true;
            str1++;
        }
        else if (found_dot)
        {
            str2++;
            found_dot = false;
        }
        else if (*str1 != *str2)
        {
            return false;
        }
        else
        {
            str1++;
            str2++;
        }
    }
    return true;
}

void strapp(char *str1, char *str2, uint8_t offset, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        if (str2[i] == 0x00)
        {
            return;
        }
        str1[i + offset] = str2[i];
    }
}

void FillStrSize(char *str, char fillchar, uint8_t size)
{
    uint8_t counter = 0;
    size--;
    while (*str)
    {
        counter++;
        str++;
    }

    if (counter <= size)
    {
        for (uint8_t i = size; i > (size - counter); i--)
        {
            str[i] = str[(counter - (size - i) - 1)];
        }
        for (uint8_t i = 0; i <= (size - counter); i++)
        {
            str[i] = fillchar;
        }
    }
}

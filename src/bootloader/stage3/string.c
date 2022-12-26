#include "headers/string.h"

bool strcmp(char *str1, char *str2)
{
    if (sizeof(*str1) != sizeof(*str2))
    {
        return false;
    }
    while (*str1 && *str2)
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

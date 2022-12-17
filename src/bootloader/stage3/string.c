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

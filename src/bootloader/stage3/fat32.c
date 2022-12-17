#include "headers/fat32.h"
#include "headers/stdio.h"
#include "headers/colors.h"

void ReadAddress(uint8_t *buffer, unsigned long int address, unsigned long int amount_chars)
{
    uint8_t* pointer = (uint8_t*)address;
    for (unsigned long int i = 0; i < amount_chars; i++)
    {
        buffer[i] = pointer[i];
    }
}

void ReadParameter(FAT32_t *fat32)
{
    uint8_t buffer[50];
    ReadAddress(buffer, 0x03, 50);
    char oemid[2];
    for (uint8_t i = 0; i < 30; i++)
    {
        oemid[i] = buffer[i];
        char outputint[20];
        ConvertHexToChar(buffer[i], outputint);
        printString(outputint, White, Black);
    }
} 
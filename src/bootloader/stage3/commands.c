#include "headers/commands.h"
#include "headers/colors.h"
#include "headers/stdio.h"
#include "headers/screen.h"
#include "headers/string.h"
#include "headers/stdint.h"
#include "headers/fat32.h"
#include "headers/keyboard.h"
#include "headers/drive.h"
#include "headers/pci.h"

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
        FAT32_t fat32;
        ReadParameter(&fat32);
        printString(fat32.OEM_ID, White, Black);
    }
    // else if (strcmp(PartSplit, "devid"))
    // {
    //     char outputint[20];
    //     clearArray(outputint, 20, 0x00);
    //     uint32_t value = drive_init();
    //     outputint[0] = '0';
    //     outputint[1] = 'x';
    //     ConvertToChar((value >> 16) & 0xFFFF, 16, outputint, 2);
    //     uint8_t _lastindex = lastIndex(outputint, 20);
    //     ConvertToChar((value >> 0)  & 0xFFFF, 16, outputint, _lastindex);
    //     printString(outputint, White, Black);
    // }
    else if (strcmp(PartSplit, "partid"))
    {
        uint8_t charArrayLength = 80;
        uint8_t outputint[charArrayLength];
        clearArray(outputint, charArrayLength, 0x00);
        pci_dev *devices;
        pci_init(devices);
        for (uint8_t i = 0; i < PCI_DEV_COUNT; i++)
        {
            pci_dev dev = devices[i];
            clearArray(outputint, charArrayLength, 0x00);
            ConvertToChar(i, 10, outputint, 0);
            uint8_t _lastindex = lastIndex(outputint, charArrayLength);
            outputint[_lastindex]     = 'B';
            outputint[_lastindex + 1] = 'x';
            ConvertToChar(dev.bus, 16, outputint, _lastindex + 2);
            _lastindex = lastIndex(outputint, charArrayLength);
            outputint[_lastindex]     = ' ';
            outputint[_lastindex + 1] = 'D';
            outputint[_lastindex + 2] = 'x';
            ConvertToChar(dev.device, 16, outputint, _lastindex + 3);
            _lastindex = lastIndex(outputint, charArrayLength);
            outputint[_lastindex]     = ' ';
            outputint[_lastindex + 1] = 'F';
            outputint[_lastindex + 2] = 'x';
            ConvertToChar(dev.function, 16, outputint, _lastindex + 3);
            _lastindex = lastIndex(outputint, charArrayLength);
            outputint[_lastindex]     = ' ';
            outputint[_lastindex + 1] = 'H';
            outputint[_lastindex + 2] = 'x';
            ConvertToChar(dev.HeaderType, 16, outputint, _lastindex + 3);
            _lastindex = lastIndex(outputint, charArrayLength);
            outputint[_lastindex]     = ' ';
            outputint[_lastindex + 1] = 'B';
            outputint[_lastindex + 2] = 'C';
            outputint[_lastindex + 3] = 'x';
            ConvertToChar(dev.ClassCode, 16, outputint, _lastindex + 4);
            _lastindex = lastIndex(outputint, charArrayLength);
            outputint[_lastindex]     = ' ';
            outputint[_lastindex + 1] = 'S';
            outputint[_lastindex + 2] = 'C';
            outputint[_lastindex + 3] = 'x';
            ConvertToChar(dev.Subclass, 16, outputint, _lastindex + 4);
            _lastindex = lastIndex(outputint, charArrayLength);
            outputint[_lastindex]     = ' ';
            printString(outputint, White, Black);
            setcursornewline();
        }
        outputint[0] = '0';
        outputint[1] = 'x';
        printString(outputint, White, Black);
    }
    else if (strcmp(PartSplit, "checkPart"))
    {
        uint8_t found = 0;
        for (uint16_t i = 0; i < 32; i++)
        {
            for (uint16_t j = 0; j < 256; j++)
            {
                uint16_t value = pciCheckVendor(i, j);
                if (value != 0xFFFF)
                {
                    found = 1;
                    char outputint[20];
                    clearArray(outputint, 20, 0x00);
                    outputint[0] = 'F';
                    outputint[1] = 'o';
                    outputint[2] = 'u';
                    outputint[3] = 'n';
                    outputint[4] = 'd';
                    outputint[5] = ' ';
                    outputint[6] = '0';
                    outputint[7] = 'x';
                    ConvertToChar(i, 16, outputint, 8);
                    uint8_t _lastindex = lastIndex(outputint, 20);
                    ConvertToChar(j, 16, outputint, _lastindex);
                    _lastindex = lastIndex(outputint, 20);
                    outputint[_lastindex] = ' ';
                    printString(outputint, White, Black);
                }
            }
        }
        if (found == 0)
        {
            printString("Nothing found", White, Black);
        }
    }
    else if (strcmp(PartSplit, "register"))
    {
        // register_keyboard_callback();
    }
    else
    {
        printString("Command not now implemented please again.", White, Black);
    }
}

void printDevID(uint8_t busid)
{
    // char outputint[20];
    // uint32_t value;
    // uint8_t _lastindex;
    // for (uint16_t i = busid; i < (busid + 8); i++)
    // {
    //     for (uint8_t j = 0; j < 8; j++)
    //     {
    //         value = pciDevice(i, j);
    //         clearArray(outputint, 20, 0x00);
    //         ConvertToChar(((i * 8) + j), 10, outputint, 0);
    //         _lastindex = lastIndex(outputint, 20);
    //         outputint[_lastindex + 0] = 'x';
    //         ConvertToChar(((value >> 16) & 0xFFFF), 16, outputint, _lastindex + 1);
    //         _lastindex = lastIndex(outputint, 20);
    //         outputint[_lastindex + 0] = ':';
    //         ConvertToChar(((value >> 0) & 0xFFFF), 16, outputint, _lastindex + 1);
    //         _lastindex = lastIndex(outputint, 20);
    //         outputint[_lastindex] = ' ';
    //         printString(outputint, White, Black);
    //     }
    // }
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

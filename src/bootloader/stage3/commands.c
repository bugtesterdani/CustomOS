#include "headers/commands.h"
#include "headers/colors.h"
#include "headers/stdio.h"
#include "headers/screen.h"
#include "headers/string.h"
#include "headers/stdint.h"
#include "headers/fat32.h"
#include "headers/keyboard.h"
#include "headers/pci.h"
#include "headers/pci_definitions.h"
#include "headers/ata.h"
#include "headers/RAM.h"
#include "headers/elf.h"
#include "headers/break.h"

#include "headers/memory_management.h"

void ParseCommand(char* commandline)
{
    printString("Received ", White, Black);
    printString(commandline, White, Black);
    setcursornewline();
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
        ReadParameter(&fat32, 0);
        if (fat32->OEM_ID[0] != 0)
        {
            printString(fat32->OEM_ID, White, Black);
            setcursornewline();
        }
        else
        {
            printString("Something went wrong", White, Black);
        }
    }
    else if (strcmp(PartSplit, "lsfs"))
    {
        FAT_Folder_t FolderStruct[10];
        uint32_t count_folders = 0;
        GetListOfFiles(0, FolderStruct, &count_folders);
        for (uint64_t i = 0; i < count_folders; i++)
        {
            uint8_t print_nane[12];
            clearArray(print_nane, 12, 0x00);
            memcp(FolderStruct[i].NAME, print_nane, 0, 11, 0);
            printString(print_nane, White, Black);
            setcursornewline();
        }
    }
    else if (strcmp(PartSplit, "outpmem"))
    {
        // Funktioniert nicht ganz so wie gedacht.
        uint8_t output[80];
        uint8_t* memspace_counter = (uint8_t*)0x500;
        memory_table_t *memory = (memory_table_t*)0x510;
        for (uint8_t i = 0; i < memspace_counter[0]; i++)
        {
            clearArray(output, 80, 0x00);
            ConvertToChar((memory[i].memory_start >> 16) & 0xFFFF, 16, output, 0);
            uint8_t _lastindex = lastIndex(output, 80);
            ConvertToChar((memory[i].memory_start >> 0)  & 0xFFFF, 16, output, _lastindex);
            _lastindex = lastIndex(output, 80);
            output[_lastindex] = ' ';
            ConvertToChar((memory[i].memory_size  >> 16) & 0xFFFF, 16, output, _lastindex + 1);
            _lastindex = lastIndex(output, 80);
            ConvertToChar((memory[i].memory_size  >> 0)  & 0xFFFF, 16, output, _lastindex);
            printString(output, White, Black);
            setcursornewline();
        }
    }
    else if (strcmp(PartSplit, "ata"))
    {
        ATA_t *atadevices;
        uint8_t *count;
        ATA_Init(&atadevices, &count);
        uint8_t charArrayLength = 80;
        uint8_t outputint[charArrayLength];
        clearArray(outputint, charArrayLength, 0x00);
        strapp(outputint, "Found ", 0, charArrayLength);
        uint8_t _lastindex = lastIndex(outputint, charArrayLength);
        ConvertToChar(count[0], 16, outputint, _lastindex);
        _lastindex = lastIndex(outputint, charArrayLength);
        // ConvertToChar(((atadevices[0].port >> 16) & 0xFFFF), 16, outputint, _lastindex);
        // _lastindex = lastIndex(outputint, charArrayLength);
        // ConvertToChar(((atadevices[0].port >>  0) & 0xFFFF), 16, outputint, _lastindex);
        // _lastindex = lastIndex(outputint, charArrayLength);
        strapp(outputint, " Hard Drive Disks", _lastindex, charArrayLength - _lastindex);
        printString(outputint, White, Black);
    }
    else if (strcmp(PartSplit, "partid"))
    {
        uint8_t charArrayLength = 80;
        uint8_t outputint[charArrayLength];
        pci_dev_t devices[PCI_DEV_COUNT];
        pci_init(devices);
        for (uint8_t i = 0; i < PCI_DEV_COUNT; i++)
        {
            pci_dev_t dev = devices[i];
            if (dev.vendorID == 0 &&
                dev.deviceID == 0)
            {
                continue;
            }
            clearArray(outputint, charArrayLength, 0x00);
            ConvertToChar(i, 10, outputint, 0);
            uint8_t _lastindex = lastIndex(outputint, charArrayLength);
            outputint[_lastindex] = ' ';
            ConvertToChar(dev.vendorID, 16, outputint, _lastindex + 1);
            _lastindex = lastIndex(outputint, charArrayLength);
            outputint[_lastindex] = ':';
            ConvertToChar(dev.deviceID, 16, outputint, _lastindex + 1);
            _lastindex = lastIndex(outputint, charArrayLength);
            outputint[_lastindex] = ' ';
            strapp(outputint, pci_returnDetails(&dev), (_lastindex + 1), (charArrayLength - _lastindex - 1));
            printString(outputint, White, Black);
            if (i + 1 < PCI_DEV_COUNT)
            {
                setcursornewline();
            }
            if (dev.ClassCode == 0x01 && dev.Subclass == 0x01)
            {
                pci_baddress_t baseaddr;
                pci_getBaseAddress(dev.bus, dev.device, dev.function, &baseaddr);
                clearArray(outputint, charArrayLength, 0x00);
                outputint[0] = '0';
                outputint[1] = 'x';
                ConvertToChar(((baseaddr.BaseAddress0 >> 16) & 0xFFFF), 16, outputint, 2);
                _lastindex = lastIndex(outputint, charArrayLength);
                ConvertToChar(((baseaddr.BaseAddress0 >>  0) & 0xFFFF), 16, outputint, _lastindex);
                _lastindex = lastIndex(outputint, charArrayLength);
                outputint[_lastindex] = ' ';
                outputint[_lastindex + 1] = '0';
                outputint[_lastindex + 2] = 'x';
                ConvertToChar(((baseaddr.BaseAddress1 >> 16) & 0xFFFF), 16, outputint, _lastindex + 3);
                _lastindex = lastIndex(outputint, charArrayLength);
                ConvertToChar(((baseaddr.BaseAddress1 >>  0) & 0xFFFF), 16, outputint, _lastindex);
                _lastindex = lastIndex(outputint, charArrayLength);
                outputint[_lastindex] = ' ';
                outputint[_lastindex + 1] = '0';
                outputint[_lastindex + 2] = 'x';
                ConvertToChar(((baseaddr.BaseAddress2 >> 16) & 0xFFFF), 16, outputint, _lastindex + 3);
                _lastindex = lastIndex(outputint, charArrayLength);
                ConvertToChar(((baseaddr.BaseAddress2 >>  0) & 0xFFFF), 16, outputint, _lastindex);
                _lastindex = lastIndex(outputint, charArrayLength);
                outputint[_lastindex] = ' ';
                outputint[_lastindex + 1] = '0';
                outputint[_lastindex + 2] = 'x';
                ConvertToChar(((baseaddr.BaseAddress3 >> 16) & 0xFFFF), 16, outputint, _lastindex + 3);
                _lastindex = lastIndex(outputint, charArrayLength);
                ConvertToChar(((baseaddr.BaseAddress3 >>  0) & 0xFFFF), 16, outputint, _lastindex);
                _lastindex = lastIndex(outputint, charArrayLength);
                outputint[_lastindex] = ' ';
                outputint[_lastindex + 1] = '0';
                outputint[_lastindex + 2] = 'x';
                ConvertToChar(((baseaddr.BaseAddress4 >> 16) & 0xFFFF), 16, outputint, _lastindex + 3);
                _lastindex = lastIndex(outputint, charArrayLength);
                ConvertToChar(((baseaddr.BaseAddress4 >>  0) & 0xFFFF), 16, outputint, _lastindex);
                _lastindex = lastIndex(outputint, charArrayLength);
                outputint[_lastindex] = ' ';
                outputint[_lastindex + 1] = '0';
                outputint[_lastindex + 2] = 'x';
                ConvertToChar(((baseaddr.BaseAddress5 >> 16) & 0xFFFF), 16, outputint, _lastindex + 3);
                _lastindex = lastIndex(outputint, charArrayLength);
                ConvertToChar(((baseaddr.BaseAddress5 >>  0) & 0xFFFF), 16, outputint, _lastindex);
                printString(outputint, White, Black);
                setcursornewline();
            }
        }
    }
    else if (strcmp(PartSplit, "partiall"))
    {
        uint8_t charArrayLength = 80;
        uint8_t outputint[charArrayLength];
        pci_dev_t devices[PCI_DEV_COUNT];
        pci_init(devices);
        for (uint8_t i = 0; i < PCI_DEV_COUNT; i++)
        {
            pci_dev_t dev = devices[i];
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
            outputint[_lastindex + 1] = 'V';
            outputint[_lastindex + 2] = 'x';
            ConvertToChar(dev.vendorID, 16, outputint, _lastindex + 3);
            _lastindex = lastIndex(outputint, charArrayLength);
            outputint[_lastindex]     = ' ';
            outputint[_lastindex + 1] = 'D';
            outputint[_lastindex + 2] = 'x';
            ConvertToChar(dev.deviceID, 16, outputint, _lastindex + 3);
            _lastindex = lastIndex(outputint, charArrayLength);
            outputint[_lastindex]     = ' ';
            printString(outputint, White, Black);
            setcursornewline();
        }
    }
    else if (strcmp(PartSplit, "load"))
    {
        clearArray(PartSplit, 60, 0x00);
        spos++;
        SplitParameters(commandline, &spos, PartSplit);
        FAT_Folder_t FolderStruct[10];
        char output[80];
        clearArray(output, 80, 0x00);
        ConvertToChar(((uint32_t)(FolderStruct)) >> 16 & 0xFFFF, 16, output, 0);
        uint8_t _lastindex = lastIndex(output, 80);
        output[_lastindex] = ' ';
        ConvertToChar(((uint32_t)(FolderStruct)) >>  0 & 0xFFFF, 16, output, _lastindex + 1);
        printString(output, White, Black);
        setcursornewline();
        uint32_t count_folders = 0;
        GetListOfFiles(0, FolderStruct, &count_folders);
        uint32_t i;
        uint8_t found = 0;
        for (i = 0; i < count_folders; i++)
        {
            uint8_t outpname[12];
            clearArray(outpname, 12, 0x00);
            memcp(FolderStruct[i].NAME, outpname, 0, 11, 0);
            found = cmplsname(outpname, PartSplit);
            if (found == 1)
            {
                printString("File Found", White, Black);
                setcursornewline();
                break;
            }
        }
        if (found == 1)
        {
            uint32_t LBA_Bytes = FolderStruct[i].HighBytes_Cluster[1] << 24 | FolderStruct[i].HighBytes_Cluster[0] << 16 |
                                 FolderStruct[i].LowBytes_Cluster[1]  <<  8 | FolderStruct[i].LowBytes_Cluster[0]  <<  0;
            uint32_t SizeBytes = (uint32_t)((*((uint32_t*)(&(*FolderStruct[i].FileSize_Byte))) & 0xFFFFFFFF) << 0);
            uint32_t address = 0;
            uint32_t offset = 0;
            uint8_t _lastindex = lastIndex(output, 80);
            if (allocate_block(&address, &offset, SizeBytes * 8) == 0)
            {
                printString("Not enough memory to allocate for ", White, Black);
            }
            clearArray(output, 80, 0x00);
            _lastindex = lastIndex(output, 80);
            ConvertToChar(((SizeBytes * 8) >> 16) & 0xFFFF, 16, output, _lastindex);
            _lastindex = lastIndex(output, 80);
            ConvertToChar(((SizeBytes * 8) >>  0) & 0xFFFF, 16, output, _lastindex);
            printString(output, White, Black);
            setcursornewline();
            clearArray(output, 80, 0x00);
            _lastindex = lastIndex(output, 80);
            ConvertToChar(((SizeBytes) >> 16) & 0xFFFF, 16, output, _lastindex);
            _lastindex = lastIndex(output, 80);
            ConvertToChar(((SizeBytes) >>  0) & 0xFFFF, 16, output, _lastindex);
            printString(output, White, Black);
            setcursornewline();
            uint16_t *address_blocked;
            address_blocked = (uint16_t*)(address + offset);

            // clearArray(output, 80, 0x00);
            // ConvertToChar(((uint32_t)(address + offset)) >> 16 & 0xFFFF, 16, output, 0);
            // _lastindex = lastIndex(output, 80);
            // output[_lastindex] = ' ';
            // ConvertToChar(((uint32_t)(address + offset)) >>  0 & 0xFFFF, 16, output, _lastindex + 1);
            // printString(output, White, Black);
            // setcursornewline();
            // clearArray(output, 80, 0x00);
            // ConvertToChar(((uint32_t)(address_blocked)) >> 16 & 0xFFFF, 16, output, 0);
            // _lastindex = lastIndex(output, 80);
            // output[_lastindex] = ' ';
            // ConvertToChar(((uint32_t)(address_blocked)) >>  0 & 0xFFFF, 16, output, _lastindex + 1);
            // printString(output, White, Black);
            // setcursornewline();

            ReadFile(0, LBA_Bytes, SizeBytes, address_blocked);

            parseELFFile(address_blocked, SizeBytes);
            breakpoint();

            unblock_space(&address, SizeBytes * 8);
        }
    }
    else if (strcmp(PartSplit, "register"))
    {
        //register_keyboard_callback();
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

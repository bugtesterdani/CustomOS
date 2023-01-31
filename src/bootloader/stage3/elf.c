#include "headers/elf.h"
#include "headers/Process.h"

#include "headers/colors.h"
#include "headers/screen.h"
#include "headers/string.h"
#include "headers/stdio.h"
#include "headers/commands.h"
#include "headers/fat32.h"
#include "headers/memory_management.h"

uint8_t parseELFFile(uint16_t *elffile, uint32_t sizeBytes, uint16_t virtual_address)
{
    // // Check if its an ELF File
    // if (((uint8_t*)elffile)[0] != 0x7F &&
    //     ((uint8_t*)elffile)[1] != 0x45 &&
    //     ((uint8_t*)elffile)[2] != 0x4c &&
    //     ((uint8_t*)elffile)[3] != 0x46)
    // {
    //     printString("Error Parsing 1", White, Black);
    //     return 0;
    // }

    // // Check if its an executable file
    // if (((uint8_t*)elffile)[10] != 0x02 &&
    //     ((uint8_t*)elffile)[11] != 0x00)
    // {
    //     printString("Error Parsing 2", White, Black);
    //     return 0;
    // }

    ELF_header_t *ELF = (ELF_header_t*)((uint32_t)elffile);
    if (*((uint32_t*)((uint32_t)(ELF->MagicNumber))) != 0x464C457F)
    {
        printString("Error Parsing 1", White, Black);
        return 0;
    }

    if (*((uint16_t*)((uint32_t)&(ELF->FileType))) != 0x0002)
    {
        printString("Error Parsing 2", White, Black);
        return 0;
    }

    uint32_t offsetBytes = ((uint32_t*)elffile)[14];
    uint32_t length = ((uint32_t*)elffile)[17];

    printString("Successfully parsed ELF file", White, Black);

    ProcessExecutor(&offsetBytes, &length, elffile, virtual_address);

    printString("Finished executing ELF file", White, Black);
    return 1;
}

void Parsing(char *name, uint32_t address_offset)
{
    FAT_Folder_t FolderStruct[10];
    uint32_t count_folders = 0;
    GetListOfFiles(0, FolderStruct, &count_folders);
    uint32_t i = 0;
    uint8_t found = 0;
    while ((i < count_folders) & (found == 0))
    {
        uint8_t outpname[12];
        clearArray(outpname, 12, 0x00);
        memcp((uint32_t*)(FolderStruct[i].NAME), (uint32_t*)outpname, 0, 3, 0);
        outpname[11] = 0;
        found = cmplsname(outpname, name);
        i++;
    }

    if (found == 1)
    {
        printString("File Found", White, Black);
        setcursornewline();
        uint32_t LBA_Bytes = FolderStruct[i].HighBytes_Cluster[1] << 24 | FolderStruct[i].HighBytes_Cluster[0] << 16 |
                                FolderStruct[i].LowBytes_Cluster[1]  <<  8 | FolderStruct[i].LowBytes_Cluster[0]  <<  0;
        uint32_t SizeBytes = (uint32_t)((*((uint32_t*)(&(*FolderStruct[i].FileSize_Byte))) & 0xFFFFFFFF) << 0);
        uint32_t address = 0;
        uint32_t offset = 0;
        if (allocate_block(&address, &offset, SizeBytes * 8) == 0)
        {
            printString("Not enough memory to allocate", White, Black);
            return;
        }
        uint16_t *address_blocked;
        address_blocked = (uint16_t*)(address + offset);

        ReadFile(0, LBA_Bytes, SizeBytes, address_blocked);
        parseELFFile(address_blocked, SizeBytes, address_offset);
        // unblock_space(&address, SizeBytes * 8);
    }
    else
    {
        printString("No File Found", White, Black);
        setcursornewline();
    }
}

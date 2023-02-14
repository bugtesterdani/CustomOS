#include "headers/elf.h"
#include "headers/Process.h"
#include "headers/paging.h"

#include "headers/colors.h"
#include "headers/screen.h"
#include "headers/string.h"
#include "headers/stdio.h"
#include "headers/commands.h"
#include "headers/fat32.h"
#include "headers/memory_management.h"

void PreparePages(ELF_header_t *ELF, ELF_header_table_t *header_table, uint32_t *start_address);

uint8_t parseELFFile(uint16_t *elffile, uint32_t sizeBytes)
{
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

    uint32_t offsetBytes = ((uint32_t*)(&(ELF->ProgramEntryPosition)))[0];
    uint32_t headertable_address = (uint32_t)((ELF->HeaderTablePosition[3] << 24) | (ELF->HeaderTablePosition[2] << 16) |
                                              (ELF->HeaderTablePosition[1] <<  8) | (ELF->HeaderTablePosition[0] <<  0));
    ELF_header_table_t *header_table = ((ELF_header_table_t*)(((uint32_t)ELF) + headertable_address));
    uint32_t start_address = 0;

    PreparePages(ELF, header_table, &start_address);

    printString("Successfully parsed ELF file", White, Black);
    uint32_t addr = 0x00;
    map_page(&addr, &addr, 0x7);
    addr = 0xbf000000;
    uint32_t block_addr = 0, block_offset = 0;
    uint8_t size = 1;
    allocate_block(&block_addr, &block_offset, (0x1000 * size));
    addr -= 0x1000;
    uint32_t block_address = (block_addr + block_offset);
    for (uint32_t i = 0; i < size; i++)
    {
        map_page(&block_address, &addr, 0x7);
        addr -= 0x1000;
        block_addr += 0x1000;
    }
    ProcessExecutor((uint32_t*)0x00);

    printString("Finished executing ELF file", White, Black);
    return 1;
}

void PreparePages(ELF_header_t *ELF, ELF_header_table_t *header_table, uint32_t *start_address)
{
    header_table = (ELF_header_table_t*)((uint32_t)header_table - ELF->SizePerProgramHeaderTableEntry);
    uint32_t start_offset = 0, save_address = 0, size = 0;
    uint32_t block_addr = 0, block_offset = 0, block_address = 0;

    for (uint32_t i = 0; i < ELF->NumberOfProgramHeaderTableEntries; i++)
    {
        uint8_t output[80];
        header_table = (ELF_header_table_t*)((uint32_t)header_table + ELF->SizePerProgramHeaderTableEntry);
        start_offset = ((uint32_t*)((header_table->p_align)))[0];
        save_address = ((uint32_t*)((header_table->p_phys_addr)))[0];
        size = ((uint32_t*)((header_table->p_mem_size)))[0];
        if (allocate_block(&block_addr, &block_offset, size) != 1)
        {
            return 0;
        }
        clearArray(output, 80, 0x00);
        printString("CP FROM: ", White, Black);
        ConvertToChar((((uint32_t)ELF) >> 16) & 0xFFFF, 16, output, 0);
        ConvertToChar((((uint32_t)ELF) >>  0) & 0xFFFF, 16, output, lastIndex(output, 80));
        printString(output, White, Black);
        setcursornewline();
        clearArray(output, 80, 0x00);
        printString("CP TO: ", White, Black);
        ConvertToChar(((block_addr + block_offset - start_offset) >> 16) & 0xFFFF, 16, output, 0);
        ConvertToChar(((block_addr + block_offset - start_offset) >>  0) & 0xFFFF, 16, output, lastIndex(output, 80));
        printString(output, White, Black);
        setcursornewline();
        clearArray(output, 80, 0x00);
        printString("OFFSET: ", White, Black);
        ConvertToChar((start_offset >> 16) & 0xFFFF, 16, output, 0);
        ConvertToChar((start_offset >>  0) & 0xFFFF, 16, output, lastIndex(output, 80));
        printString(output, White, Black);
        setcursornewline();
        clearArray(output, 80, 0x00);
        printString("SIZE: ", White, Black);
        ConvertToChar(((size) >> 16) & 0xFFFF, 16, output, 0);
        ConvertToChar(((size) >>  0) & 0xFFFF, 16, output, lastIndex(output, 80));
        printString(output, White, Black);
        setcursornewline();
        memcp(((uint8_t*)((uint32_t)ELF)), (uint8_t*)(block_addr + block_offset - start_offset), start_offset, (start_offset + size), 0);
        if (i == 0)
        {
            *start_address = (block_addr + block_offset);
        }

        block_address = (block_addr + block_offset);

        map_page((&block_address), &save_address, 0x7);
    }
    return 1;
}

void Parsing(char *name)
{
    FAT_Folder_t FolderStruct[10];
    uint32_t count_folders = 0;
    GetListOfFiles(0, FolderStruct, &count_folders);
    uint32_t i = 0;
    uint8_t found = 0;

    // Load the Paging enabler and jumper to 0xFF000000
    uint8_t loader_bytes[27];
    loader_bytes[0] = 0x0F;
    loader_bytes[1] = 0x20;
    loader_bytes[2] = 0xC0;
    loader_bytes[3] = 0x0D;
    loader_bytes[4] = 0x01;
    loader_bytes[5] = 0x00;
    loader_bytes[6] = 0x00;
    loader_bytes[7] = 0x80;
    loader_bytes[8] = 0x0F;
    loader_bytes[9] = 0x22;
    loader_bytes[10] = 0xC0;
    loader_bytes[11] = 0xBC;
    loader_bytes[12] = 0x00;
    loader_bytes[13] = 0x00;
    loader_bytes[14] = 0x00;
    loader_bytes[15] = 0xBF;
    loader_bytes[16] = 0x89;
    loader_bytes[17] = 0xE5;
    loader_bytes[18] = 0xB8;
    loader_bytes[19] = 0x00;
    loader_bytes[20] = 0x00;
    loader_bytes[21] = 0x00;
    loader_bytes[22] = 0xBF;
    loader_bytes[23] = 0xFF;
    loader_bytes[24] = 0xE0;
    loader_bytes[25] = 0xC3;
    loader_bytes[26] = 0x00;
    memcp(loader_bytes, (uint8_t*)0x00, 0, 27, 0);

    while ((i < count_folders) & (found == 0))
    {
        uint8_t outpname[12];
        clearArray(outpname, 12, 0x00);
        memcp(FolderStruct[i].NAME, outpname, 0, 11, 0);
        outpname[11] = 0;
        printString(outpname, White, Black);
        setcursornewline();
        found = cmplsname(outpname, name);
        if (found == 0)
        {
            i++;
        }
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
        memset((uint8_t*)address_blocked, SizeBytes, 0x00);
        ReadFile(0, LBA_Bytes, SizeBytes * 8, address_blocked);

        uint8_t output[80];
        clearArray(output, 80, 0x00);
        printString("ADDR PS1: ", White, Black);
        ConvertToChar((((uint32_t)(address_blocked)) >> 16) & 0xFFFF, 16, output, 0);
        ConvertToChar((((uint32_t)(address_blocked)) >>  0) & 0xFFFF, 16, output, lastIndex(output, 80));
        printString(output, White, Black);
        setcursornewline(); 
        clearArray(output, 80, 0x00);
        printString("LBA: ", White, Black);
        ConvertToChar((((uint32_t)(LBA_Bytes)) >> 16) & 0xFFFF, 16, output, 0);
        ConvertToChar((((uint32_t)(LBA_Bytes)) >>  0) & 0xFFFF, 16, output, lastIndex(output, 80));
        printString(output, White, Black);
        setcursornewline(); 
        clearArray(output, 80, 0x00);
        printString("SIZE PS2: ", White, Black);
        ConvertToChar(((SizeBytes) >> 16) & 0xFFFF, 16, output, 0);
        ConvertToChar(((SizeBytes) >>  0) & 0xFFFF, 16, output, lastIndex(output, 80));
        printString(output, White, Black);
        setcursornewline(); 

        parseELFFile(address_blocked, SizeBytes);
        // unblock_space(&address, SizeBytes * 8);
    }
    else
    {
        printString("No File Found", White, Black);
        setcursornewline();
    }
}

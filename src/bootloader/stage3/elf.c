#include "headers/elf.h"
#include "headers/Process.h"
#include "headers/paging.h"

#include "headers/colors.h"
#include "headers/screen.h"
#include "headers/string.h"
#include "headers/stdio.h"
#include "headers/fat32.h"
#include "headers/memory_management.h"

uint8_t PreparePages(ELF_header_t *ELF, ELF_header_table_t *header_table);

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

    uint32_t headertable_address = (uint32_t)((ELF->HeaderTablePosition[3] << 24) | (ELF->HeaderTablePosition[2] << 16) |
                                              (ELF->HeaderTablePosition[1] <<  8) | (ELF->HeaderTablePosition[0] <<  0));
    ELF_header_table_t *header_table = ((ELF_header_table_t*)(((uint32_t)ELF) + headertable_address));
    
    if (PreparePages(ELF, header_table) == 0)
    {
        return 0;
    }

    uint32_t start_address = (ELF->ProgramEntryPosition[3] << 24) +
                             (ELF->ProgramEntryPosition[2] << 16) +
                             (ELF->ProgramEntryPosition[1] <<  8) +
                             (ELF->ProgramEntryPosition[0] <<  0);

    unblock_space((uint32_t*)elffile, (uint64_t)sizeBytes);
    memset((uint8_t*)elffile, sizeBytes, 0);

    printString("Successfully parsed ELF file", White, Black);

    // Map the Switch 0x00 Code part to page address 0x00
    uint32_t addr = 0x00;
    map_page(&addr, &addr, 0x7);

    // Map the Stack Page from 0xBF000000 down with size Pages
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

    // Map the Space where we can later save our static Values in the new Kernel
    block_addr = 0;
    block_offset = 0;
    addr = 0xc0000000;
    uint32_t end_addr = 0xc000f000;
    allocate_block(&block_addr, &block_offset, (end_addr - addr));
    size = ((end_addr - addr) / 0x1000);
    block_address = (block_addr + block_offset);
    for (uint32_t i = 0; i < size; i++)
    {
        map_page(&block_address, &addr, 0x7);
        addr += 0x1000;
        block_addr += 0x1000;
    }

    // Map the Output Graphic Addresses to 0xB00000000
    addr = 0xb0000000;
    block_address = 0xB8000;
    size = 100;
    for (uint32_t i = 0; i < size; i++)
    {
        map_page(&block_address, &addr, 0x7);
        addr += 0x1000;
        block_addr += 0x1000;
    }

    ProcessExecutor((uint32_t*)0x00, start_address);

    printString("Finished executing ELF file", White, Black);
    return 1;
}

uint8_t PreparePages(ELF_header_t *ELF, ELF_header_table_t *header_table)
{
    header_table = (ELF_header_table_t*)((uint32_t)header_table - ELF->SizePerProgramHeaderTableEntry);
    uint32_t start_offset = 0, save_address = 0, size = 0;
    uint32_t block_addr = 0, block_offset = 0, block_address = 0;

    for (uint32_t i = 0; i < ELF->NumberOfProgramHeaderTableEntries; i++)
    {
        header_table = (ELF_header_table_t*)((uint32_t)header_table + ELF->SizePerProgramHeaderTableEntry);
        start_offset = ((uint32_t*)((header_table->p_align)))[0];
        save_address = ((uint32_t*)((header_table->p_phys_addr)))[0];
        size = ((uint32_t*)((header_table->p_mem_size)))[0];
        if (allocate_block(&block_addr, &block_offset, size) != 1)
        {
            return 0;
        }
        // char output[80];
        // clearArray((uint8_t*)output, 80, 0x00);
        // ConvertToChar(((uint32_t)((uint8_t*)((uint32_t*)ELF)) >> 16) & 0xFFFF, 16, output, 0);
        // ConvertToChar(((uint32_t)((uint8_t*)((uint32_t*)ELF)) >>  0) & 0xFFFF, 16, output, lastIndex(output, 80));
        // printString(output, White, Black);
        // setcursornewline();
        // clearArray((uint8_t*)output, 80, 0x00);
        // ConvertToChar(((uint32_t)((uint8_t*)(block_addr + block_offset - start_offset)) >> 16) & 0xFFFF, 16, output, 0);
        // ConvertToChar(((uint32_t)((uint8_t*)(block_addr + block_offset - start_offset)) >>  0) & 0xFFFF, 16, output, lastIndex(output, 80));
        // printString(output, White, Black);
        // setcursornewline();
        // clearArray((uint8_t*)output, 80, 0x00);
        // ConvertToChar((start_offset >> 16) & 0xFFFF, 16, output, 0);
        // ConvertToChar((start_offset >>  0) & 0xFFFF, 16, output, lastIndex(output, 80));
        // printString(output, White, Black);
        // setcursornewline();
        // clearArray((uint8_t*)output, 80, 0x00);
        // ConvertToChar(((start_offset + size) >> 16) & 0xFFFF, 16, output, 0);
        // ConvertToChar(((start_offset + size) >>  0) & 0xFFFF, 16, output, lastIndex(output, 80));
        // printString(output, White, Black);
        // setcursornewline();
        memcp(((uint8_t*)((uint32_t*)ELF)), (uint8_t*)(block_addr + block_offset - start_offset), start_offset, (start_offset + size), 0);

        block_address = (block_addr + block_offset);

        uint32_t counter = (size / 0x1000);
        if (size % 0x1000 > 0)
        {
            counter++;
        }

        for (uint32_t i = 0; i < counter; i++)
        {
            map_page((&block_address), &save_address, 0x7);
            save_address += 0x1000;
            block_address += 0x1000;
        }
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
    uint32_t loader_bytes[6];
    loader_bytes[0] = 0x0F0C5D8B;
    loader_bytes[1] = 0x010DC020;
    loader_bytes[2] = 0x0F800000;
    loader_bytes[3] = 0x00BCC022;
    loader_bytes[4] = 0x89BF0000;
    loader_bytes[5] = 0xC3E3FFE5;
    memcp((uint8_t*)loader_bytes, (uint8_t*)0x00, 0, 6 * 4, 0);

    while ((i < count_folders) & (found == 0))
    {
        char outpname[12];
        clearArray((uint8_t*)outpname, 12, 0x00);
        memcp(FolderStruct[i].NAME, (uint8_t*)outpname, 0, 11, 0);
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
        char output[80];
        clearArray((uint8_t*)output, 80, 0x00);
        ConvertToChar(((SizeBytes) >> 16) & 0xFFFF, 16, output, 0);
        ConvertToChar(((SizeBytes) >>  0) & 0xFFFF, 16, output, lastIndex(output, 80));
        printString(output, White, Black);
        setcursornewline();
        if (allocate_block(&address, &offset, SizeBytes) == 0)
        {
            printString("Not enough memory to allocate", White, Black);
            return;
        }
        uint16_t *address_blocked;
        address_blocked = (uint16_t*)(address + offset);
        memset((uint8_t*)address_blocked, SizeBytes, 0x00);
        ReadFile(0, LBA_Bytes, SizeBytes, address_blocked);

        parseELFFile(address_blocked, SizeBytes);
        // unblock_space(&address, SizeBytes * 8);
    }
    else
    {
        printString("No File Found", White, Black);
        setcursornewline();
    }
}

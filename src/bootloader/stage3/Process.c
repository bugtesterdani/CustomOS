#include "headers/Process.h"
#include "headers/memory_management.h"
#include "headers/paging.h"

static uint16_t currentPID = 0;
typedef void func(void);

void ProcessExecutor(uint32_t *offset, uint32_t *length, uint16_t *file, uint16_t virtual_address)
{
    // uint32_t blocks, page_table;
    // currentPID = currentPID + 1;
    // blocks = *length / 4096;
    // uint32_t address, addr_offset;
    // allocate_block(&address, &addr_offset, *length);
    // uint32_t *address_directory;
    // paging_setup_newDirectory(address_directory, 1);
    // for (uint16_t i = 0; i <= ((blocks - 1) / 1024); i++)
    // {
    //     uint32_t *table_address = ((uint32_t*)(address_directory[i]));
    //     for (uint16_t j = 0; j < blocks % 1024; j++)
    //     {
    //         table_address[j] = ((address + addr_offset) | 0x03);
    //     }
    // }
    if (paging_setup_newTable(0) != 1)
    {
        return;
    }
    uint32_t address, addr_offset;
    if (allocate_block(&address, &addr_offset, (uint64_t)*length) != 1)
    {
        return;
    }

    memcp((uint32_t*)file, (uint32_t*)(address + addr_offset), *offset, (*length / 32), 0);

    for (uint32_t i = 0; i < *length; i += BLOCK_SIZE)
    {
        uint32_t *program_address = (uint32_t*)(address + addr_offset + i);
        uint32_t *virtual_address = (uint32_t*)virtual_address + i;
        map_page(program_address, virtual_address, 0x07);
    }
    func* program = (func*)(address + addr_offset);
    program();
}

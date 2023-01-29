#include "headers/Process.h"
#include "headers/memory_management.h"
#include "headers/paging.h"

static uint16_t currentPID = 0;

void ProcessExecutor(uint32_t *offset, uint32_t *length, uint16_t *file)
{
    return;
    uint32_t blocks, page_table;
    currentPID = currentPID + 1;
    blocks = *length / 4096;
    uint32_t address, addr_offset;
    allocate_block(&address, &addr_offset, *length);
    uint32_t *address_directory;
    paging_setup_newDirectory(address_directory, 1);
    for (uint16_t i = 0; i <= ((blocks - 1) / 1024); i++)
    {
        uint32_t *table_address = ((uint32_t*)(address_directory[i]));
        for (uint16_t j = 0; j < blocks % 1024; j++)
        {
            table_address[j] = ((address + addr_offset) | 0x03);
        }
    }
}

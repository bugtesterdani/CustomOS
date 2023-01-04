#include "headers/memory_management.h"

// To use 264Bit
memory_table_t *memory = (memory_table_t*)0x510;
// To use 500Bit => Support only for now 2GB with 4kBit Pages
uint32_t *memory_params = (uint32_t*)0x80C;
uint32_t* memspace_counter = (uint32_t*)0x500;

uint8_t checkRAM(RAM_t *RAM);

// Register the RAM Spaces / Addresses
void register_memspace(RAM_t *RAM)
{
    if (checkRAM(RAM) == 0)
    {
        return;
    }
    uint32_t count = memspace_counter[0];
    memory[count].memory_start = RAM->BaseAddress;
    memory[count].memory_size = RAM->Size;
    memspace_counter[0] = count + 1;
}

uint8_t checkRAM(RAM_t *RAM)
{
    uint32_t count = memspace_counter[0];
    for (uint64_t i = 0; i < count; i++)
    {
        if (RAM->BaseAddress <= memory[i].memory_start &&
            RAM->BaseAddress + RAM->Size <= memory[i].memory_start)
        {
            continue;
        }
        if (RAM->BaseAddress >= (memory[i].memory_start + memory[i].memory_size))
        {
            continue;
        }
        return 0;
    }
    return 1;
}

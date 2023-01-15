#include "headers/memory_management.h"

#include "headers/screen.h"
#include "headers/commands.h"
#include "headers/fat32.h"
#include "headers/string.h"
#include "headers/colors.h"
#include "headers/stdio.h"

// To use 264Bit
memory_table_t *memory = (memory_table_t*)0x510;
uint32_t* memspace_counter = (uint32_t*)0x500;

// Ist die selbe Addresse wie der memory pointer.
// Hier wird die erste freie Addresse für die MAP verwendet.
// In der Memory Map sind der erste 1 (uint32_t) Einträge,
// Einträge, die auskunft über die Blöcke geben.
// => 1. uint32_t Block:
//      MAX_BLOCKS
#define MEMORY_MAP_ADDRESS_OFFSET           0x004
#define MEMORY_MAP_MAX_BLOCK_OFFSET         0x000

uint8_t checkRAM(RAM_t *RAM);
uint8_t find_first_free_block(uint32_t blocks, uint32_t *block);
void set_block(uint32_t bit);
void unset_block(uint32_t bit);
void block_memory_map_space();

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
    for (uint64_t i = 0; i < memspace_counter[0]; i++)
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

// We set the memory map Address to the first free block,
// and go further to set this address to the 
uint8_t register_memory_map()
{
    // First make a offset change to the BLOCKSIZE of 4096
    uint32_t offset_align = (memory[0].memory_start % BLOCK_SIZE);
    memory[0].memory_start += offset_align;
    memory[0].memory_size -= offset_align;
    
    uint32_t *memory_map = (uint32_t*)(memory[0].memory_start + MEMORY_MAP_ADDRESS_OFFSET);

    if (memspace_counter[0] == 0)
    {
        return 0;
    }
    uint32_t *memory_max_blocks = (uint32_t*)(memory[0].memory_start + MEMORY_MAP_MAX_BLOCK_OFFSET);
    *memory_max_blocks = (memory[0].memory_size + MEMORY_MAP_ADDRESS_OFFSET) / BLOCK_SIZE;

    // set all blocks as unused
    unblock_space(0, *memory_max_blocks);
    block_memory_map_space();
    return 1;
}

void block_memory_map_space()
{
    uint32_t *memory_max_blocks = (uint32_t*)(memory[0].memory_start + MEMORY_MAP_MAX_BLOCK_OFFSET);
    uint16_t count_block_pages = (*memory_max_blocks) / BLOCK_SIZE;
    if (*memory_max_blocks % BLOCK_SIZE >= 1)
    {
        count_block_pages++;
    }
    for (uint32_t counter = 0; counter < count_block_pages; counter++)
    {
        set_block(counter);
    }
}

uint8_t allocate_block(uint32_t *address, uint32_t *offset_addr, uint64_t size)
{
    uint32_t loc_addr = 0;
    uint32_t block = size / BLOCK_SIZE;
    if ((size % BLOCK_SIZE) > 0)
    {
        block++;
    }
    printString("Received allocate block", White, Black);
    setcursornewline();

    if (find_first_free_block(block, &loc_addr) != 1)
    {
        return 0;
    }

    for (uint32_t i = 0; i < block; i++)
    {
        set_block(loc_addr + i);
    }

    *address = (loc_addr * BLOCK_SIZE);
    *offset_addr = memory[0].memory_start;
    clearArray(((uint8_t*)(*address + *offset_addr)), (block * BLOCK_SIZE), 0x00);

    printString("done allocate block", White, Black);
    setcursornewline();
    return 1;
}

void output_mem_things()
{
    uint8_t output[80];
    printString("Memory Start: ", White, Black);
    clearArray(output, 80, 0x00);
    ConvertToChar((memory[0].memory_start >> 16) & 0xFFFF, 16, output, 0);
    uint8_t _lastindex = lastIndex(output, 80);
    output[_lastindex] = ' ';
    ConvertToChar((memory[0].memory_start >>  0) & 0xFFFF, 16, output, _lastindex + 1);
    _lastindex = lastIndex(output, 80);
    output[_lastindex] = ' ';
    output[_lastindex + 1] = ' ';
    printString(output, White, Black);
    printString("Memory Start ADDR: ", White, Black);
    clearArray(output, 80, 0x00);
    ConvertToChar((((uint32_t)&(memory[0].memory_start)) >> 16) & 0xFFFF, 16, output, 0);
    _lastindex = lastIndex(output, 80);
    output[_lastindex] = ' ';
    ConvertToChar((((uint32_t)&(memory[0].memory_start)) >>  0) & 0xFFFF, 16, output, _lastindex + 1);
    _lastindex = lastIndex(output, 80);
    printString(output, White, Black);
    setcursornewline();
    printString("Memory Size: ", White, Black);
    clearArray(output, 80, 0x00);
    ConvertToChar((memory[0].memory_size >> 16) & 0xFFFF, 16, output, 0);
    _lastindex = lastIndex(output, 80);
    output[_lastindex] = ' ';
    ConvertToChar((memory[0].memory_size >>  0) & 0xFFFF, 16, output, _lastindex + 1);
    _lastindex = lastIndex(output, 80);
    output[_lastindex] = ' ';
    output[_lastindex + 1] = ' ';
    printString(output, White, Black);
    printString("Memory Size ADDR: ", White, Black);
    clearArray(output, 80, 0x00);
    ConvertToChar((((uint32_t)&(memory[0].memory_size)) >> 16) & 0xFFFF, 16, output, 0);
    _lastindex = lastIndex(output, 80);
    output[_lastindex] = ' ';
    ConvertToChar((((uint32_t)&(memory[0].memory_size)) >>  0) & 0xFFFF, 16, output, _lastindex + 1);
    _lastindex = lastIndex(output, 80);
    printString(output, White, Black);
    setcursornewline();
}

void unblock_space(uint32_t *address, uint64_t size)
{
    uint32_t block = *address / BLOCK_SIZE;
    uint32_t blocks = size / BLOCK_SIZE;
    if ((size % BLOCK_SIZE) > 0)
    {
        blocks++;
    }

    for (uint32_t i = 0; i < blocks; i++)
    {
        unset_block(block + i);
    }
}

uint8_t find_first_free_block(uint32_t blocks, uint32_t *block)
{
    printString("Received find block", White, Black);
    setcursornewline();
    if (blocks == 0)
    {
        return 0;
    }

    uint32_t *memory_max_blocks = (uint32_t*)(memory[0].memory_start + MEMORY_MAP_MAX_BLOCK_OFFSET);
    if (*memory_max_blocks < blocks)
    {
        return 0;
    }

    uint32_t *memory_map = (uint32_t*)(memory[0].memory_start + MEMORY_MAP_ADDRESS_OFFSET);
    uint64_t free_blocks = 0;
    for (uint64_t i = 0; i < (*memory_max_blocks / 32) + 1; i++)
    {
        if (memory_map[i]== 0xFFFFFFFF)
        {
            free_blocks = 0;
            continue;
        }

        for (uint32_t j = 0; j < 32; j++)
        {
            if ((memory_map[i] & (1 << j)) >= 1)
            {
                free_blocks = 0;
                continue;
            }
            if (free_blocks == 0)
            {
                *block = ((i * 32) + j);
            }
            free_blocks++;
        }
        if (blocks <= free_blocks)
        {
            return 1;
        }
    }
    return 0;
}

void set_block(uint32_t bit)
{
    printString("Received set block", White, Black);
    setcursornewline();
    // There is a Pointer at address 0x510, which points to the start address of the map (by offset 0x008)
    uint32_t *memory_map = (uint32_t*)(memory[0].memory_start + MEMORY_MAP_ADDRESS_OFFSET);
    memory_map[bit/32] |= (1 << (bit % 32));
}

void unset_block(uint32_t bit)
{
    // There is a Pointer at address 0x510, which points to the start address of the map (by offset 0x008)
    uint32_t *memory_map = (uint32_t*)(memory[0].memory_start + MEMORY_MAP_ADDRESS_OFFSET);
    memory_map[bit/32] &= ~(1 << (bit % 32));
}

void memcp(uint8_t *src, uint8_t *dst, uint8_t start, uint8_t count, uint8_t offset_dst)
{
    for (uint16_t i = start; i < count; i++)
    {
        dst[i + offset_dst] = src[i];
    }
}

void memset(uint8_t *dst, uint32_t size, uint8_t value)
{
    for (uint64_t i = 0; i < size; i++)
    {
        dst[i] = value;
    }
}

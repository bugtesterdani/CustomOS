#include "headers/memory_management.h"

memory_table_t *memory = (memory_table_t*)0x510;
uint32_t* memspace_counter = (uint32_t*)0x500;
uint8_t checkRAM(RAM_t *RAM);

void register_memspace(RAM_t *RAM)
{
    if (checkRAM(RAM) == 0)
    {
        return;
    }
    uint32_t count = memspace_counter[0];
    memory[count].memory_start = RAM->BaseAddress;
    memory[count].memory_size = RAM->Size;
    memory[count].parameter = 0x00;
    memory[count].unique_identifier = 0x00;
    memspace_counter[0] = count + 1;
}

uint64_t memory_get_addr(uint64_t *unique_identifier)
{
    uint32_t count = unique_identifier[0] & 0xFFFFFFFF;
    if (memory[count].unique_identifier != unique_identifier)
    {
        return 0;
    }
    return memory[count].memory_start;
}

void freeup_memory(uint64_t *unique_identifier)
{
    uint32_t count = unique_identifier[0] & 0xFFFFFFFF;
    if (memory[count].unique_identifier != unique_identifier)
    {
        return;
    }

    uint32_t* _memory_pointer = (uint32_t*)memory[count].memory_start;
    for (uint64_t i = 0; i < memory[count].memory_size; i++)
    {
        _memory_pointer[i] = (uint32_t)0x00;
    }
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

uint64_t request_memory(uint64_t size_request, uint64_t *unique_identifier)
{
    uint32_t counter = memspace_counter[0];
    for (uint64_t i = 0; i < counter; i++)
    {
        if ((memory[i].parameter & 0x01) == 1)
        {
            continue;
        }
        if (memory[i].memory_size < size_request)
        {
            continue;
        }
        memory[i].memory_size -= size_request;
        memory[counter].memory_start = memory[i].memory_start + memory[i].memory_size;
        memory[counter].memory_size = size_request;
        memory[counter].parameter = 0x01;
        *unique_identifier |= counter;
        memory[counter].unique_identifier = unique_identifier;
        memspace_counter[0] = counter + 1;
        freeup_memory(unique_identifier);
        return counter;
    }
    return 0;
}

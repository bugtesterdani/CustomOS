#pragma once

#include "stdint.h"
#include "RAM.h"

struct memory_table
{
    // Bit 0: Used => 1
    //        Not Used => 0
    uint32_t parameter;
    uint32_t memory_start;
    uint32_t memory_size;
    uint64_t *unique_identifier;
} __attribute__((packed));

typedef struct memory_table memory_table_t;

void freeup_memory(uint64_t *unique_identifier);
void unblock_memory(uint64_t *unique_identifier);
uint64_t memory_get_addr(uint64_t *unique_identifier);
uint64_t request_memory(uint64_t size_request, uint64_t *unique_identifier);
void register_memspace(RAM_t *RAM);

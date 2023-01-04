#pragma once

#include "stdint.h"
#include "RAM.h"

struct memory_table
{
    uint32_t memory_start;
    uint32_t memory_size;
} __attribute__((packed));

typedef struct memory_table memory_table_t;

void register_memspace(RAM_t *RAM);

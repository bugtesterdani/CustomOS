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
void memcp(uint8_t *src, uint8_t *dst, uint8_t start, uint8_t count, uint8_t offset_dst);

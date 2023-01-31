#pragma once

#include "stdint.h"
#include "RAM.h"

#define BLOCK_SIZE      4096
#define BLOCKS_PER_BYTE 8

struct memory_table
{
    uint32_t memory_start;
    uint32_t memory_size;
} __attribute__((packed));

typedef struct memory_table memory_table_t;

uint8_t register_memory_map();
uint8_t allocate_block(uint32_t *address, uint32_t *offset_addr, uint64_t size);
void unblock_space(uint32_t *address, uint64_t size);
void block_space(uint32_t *address, uint64_t size);
void register_memspace(RAM_t *RAM);
void memcp(uint32_t *src, uint32_t *dst, uint32_t start, uint32_t count, uint32_t offset_dst);
void memset(uint8_t *dst, uint32_t size, uint8_t value);

// void output_mem_things();

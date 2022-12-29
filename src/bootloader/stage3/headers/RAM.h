#pragma once

#include "stdint.h"

struct RAM
{
    uint64_t BaseAddress;
    uint64_t Size;
    uint32_t Type;
} __attribute__((packed));

typedef struct RAM RAM_t;

void RAM_Init();
void ram_parseToCharArr(char *str, uint16_t value, uint8_t charmax);
void ram_subPrint(uint8_t size, uint64_t value);
void raminit_1();

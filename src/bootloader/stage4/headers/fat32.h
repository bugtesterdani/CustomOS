#pragma once
#include "stdint.h"
#include "x86.h"

typedef struct
{
    uint8_t amount_sectors;
    uint8_t lsb_lba1;
    uint8_t msb_lba1;
    uint8_t lsb_lba2;
    uint8_t msb_lba2;
    uint8_t lsb_lba3;
    uint8_t msb_lba3;
    uint8_t lsb_lba4;
    uint8_t msb_lba4;
    uint8_t lsb_buff_offset;
    uint8_t msb_buff_offset;
} FAT32_Read;

static FAT32_Read* fat32;
const char* ReadFatFile(FAT32_Read* fat32);

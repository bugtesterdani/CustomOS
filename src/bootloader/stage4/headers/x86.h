#pragma once
#include "stdint.h"

void _cdecl x86_PrintString(const char* s);
uint8_t* _cdecl x86_ReadFAT(
    uint8_t amount_sectors,
    uint8_t lsb_lba1,
    uint8_t msb_lba1,
    uint8_t lsb_lba2,
    uint8_t msb_lba2,
    uint8_t lsb_lba3,
    uint8_t msb_lba3,
    uint8_t lsb_lba4,
    uint8_t msb_lba4,
    uint8_t lsb_buff_offset,
    uint8_t msb_buff_offset
);

void _cdecl x86_waitKeyPress();

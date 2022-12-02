#include "headers/stdint.h"
#include "headers/fat32.h"

const char* ReadFatFile(FAT32_Read* fat32)
{
    return x86_ReadFAT(
        fat32->amount_sectors,
        fat32->lsb_lba1,
        fat32->msb_lba1,
        fat32->lsb_lba2,
        fat32->msb_lba2,
        fat32->lsb_lba3,
        fat32->msb_lba3,
        fat32->lsb_lba4,
        fat32->msb_lba4,
        fat32->lsb_buff_offset,
        fat32->msb_buff_offset
    );
}

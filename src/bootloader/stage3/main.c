#include "headers/stdint.h"
#include "headers/stdio.h"
#include "headers/fat32.h"

void _cdecl cstart_(uint16_t bootDrive)
{
    printstr("Hello from the C Code\r\n");

    fat32->amount_sectors = 1;
    fat32->lsb_lba1 = 0;
    fat32->msb_lba1 = 0;
    fat32->lsb_lba2 = 0;
    fat32->msb_lba2 = 0;
    fat32->lsb_lba3 = 0;
    fat32->msb_lba3 = 0;
    fat32->lsb_lba4 = 0;
    fat32->msb_lba4 = 0;
    fat32->lsb_buff_offset = 24;
    fat32->msb_buff_offset = 11;

    printstr(ReadFatFile(fat32));
    printstr("\r\n------\r\n");
    printstr(ReadFatFile(fat32));
    for(;;);
}

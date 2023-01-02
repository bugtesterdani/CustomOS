#include "headers/fat32.h"
#include "headers/memory_management.h"

#include "headers/stdio.h"
#include "headers/isr.h"
#include "headers/io.h"

static uint64_t fat32_unique_ident = 0x10200000000;

uint64_t ReadParameter()
{
    uint64_t memory_addr;
    if ((fat32_unique_ident & 0xFFFF) == 0x00)
    {
        memory_addr = request_memory(50, &fat32_unique_ident);
        // Something went wrong
        if (memory_addr == 0)
        {
            return 0;
        }
    }
    else
    {
        return memory_get_addr(&fat32_unique_ident);
    }

    FAT32_t* fat32 = (FAT32_t*)memory_addr;
    uint8_t sector_amount = 1;
    uint64_t outp[256 * sector_amount];
    ReadSectorsLBA(0, 0, sector_amount, outp);

    uint8_t offset_fat32 = 1;

    fat32->OEM_ID[0] = ((outp[offset_fat32 + 0] >> 0) & 0xFF);
    fat32->OEM_ID[1] = ((outp[offset_fat32 + 1] >> 8) & 0xFF);
    fat32->OEM_ID[2] = ((outp[offset_fat32 + 1] >> 0) & 0xFF);
    fat32->OEM_ID[3] = ((outp[offset_fat32 + 2] >> 8) & 0xFF);
    fat32->OEM_ID[4] = ((outp[offset_fat32 + 2] >> 0) & 0xFF);
    fat32->OEM_ID[5] = ((outp[offset_fat32 + 3] >> 8) & 0xFF);
    fat32->OEM_ID[6] = ((outp[offset_fat32 + 3] >> 0) & 0xFF);
    fat32->OEM_ID[7] = ((outp[offset_fat32 + 4] >> 8) & 0xFF);

    fat32->BytesPerSector[0] = ((outp[offset_fat32 + 4] >> 0) & 0xFF);
    fat32->BytesPerSector[1] = ((outp[offset_fat32 + 5] >> 8) & 0xFF);

    fat32->SectorsPerCluster = ((outp[offset_fat32 + 5] >> 0) & 0xFF);

    fat32->ReservedSectors[0] = ((outp[offset_fat32 + 6] >> 8) & 0xFF);
    fat32->ReservedSectors[1] = ((outp[offset_fat32 + 6] >> 0) & 0xFF);

    fat32->TotalFATs[0] = ((outp[offset_fat32 + 7] >> 8) & 0xFF);
    fat32->TotalFATs[1] = ((outp[offset_fat32 + 7] >> 0) & 0xFF);

    fat32->MaxRootEntries[0] = ((outp[offset_fat32 + 8] >> 8) & 0xFF);
    fat32->MaxRootEntries[1] = ((outp[offset_fat32 + 8] >> 0) & 0xFF);

    fat32->NumberOfSectors[0] = ((outp[offset_fat32 + 9] >> 8) & 0xFF);
    fat32->NumberOfSectors[1] = ((outp[offset_fat32 + 9] >> 0) & 0xFF);

    fat32->MediaDescriptor = ((outp[offset_fat32 + 10] >> 8) & 0xFF);

    fat32->SectorsPerFAT[0] = ((outp[offset_fat32 + 10] >> 0) & 0xFF);
    fat32->SectorsPerFAT[1] = ((outp[offset_fat32 + 11] >> 8) & 0xFF);

    fat32->SectorsPerTrack[0] = ((outp[offset_fat32 + 11] >> 0) & 0xFF);
    fat32->SectorsPerTrack[1] = ((outp[offset_fat32 + 12] >> 8) & 0xFF);

    fat32->SectorsPerHead[0] = ((outp[offset_fat32 + 12] >> 0) & 0xFF);
    fat32->SectorsPerHead[1] = ((outp[offset_fat32 + 13] >> 8) & 0xFF);

    fat32->HiddenSectors[0] = ((outp[offset_fat32 + 13] >> 0) & 0xFF);
    fat32->HiddenSectors[1] = ((outp[offset_fat32 + 14] >> 8) & 0xFF);
    fat32->HiddenSectors[2] = ((outp[offset_fat32 + 14] >> 0) & 0xFF);
    fat32->HiddenSectors[3] = ((outp[offset_fat32 + 15] >> 8) & 0xFF);

    fat32->TotalSectors[0] = ((outp[offset_fat32 + 15] >> 0) & 0xFF);
    fat32->TotalSectors[1] = ((outp[offset_fat32 + 16] >> 8) & 0xFF);
    fat32->TotalSectors[2] = ((outp[offset_fat32 + 16] >> 0) & 0xFF);
    fat32->TotalSectors[3] = ((outp[offset_fat32 + 17] >> 8) & 0xFF);

    fat32->BigSectorsPerFAT[0] = ((outp[offset_fat32 + 17] >> 0) & 0xFF);
    fat32->BigSectorsPerFAT[1] = ((outp[offset_fat32 + 18] >> 8) & 0xFF);
    fat32->BigSectorsPerFAT[2] = ((outp[offset_fat32 + 18] >> 0) & 0xFF);
    fat32->BigSectorsPerFAT[3] = ((outp[offset_fat32 + 19] >> 8) & 0xFF);

    fat32->Flags[0] = ((outp[offset_fat32 + 19] >> 0) & 0xFF);
    fat32->Flags[1] = ((outp[offset_fat32 + 20] >> 8) & 0xFF);

    fat32->FSVersion[0] = ((outp[offset_fat32 + 20] >> 0) & 0xFF);
    fat32->FSVersion[1] = ((outp[offset_fat32 + 21] >> 8) & 0xFF);

    fat32->RootDirectoryStart[0] = ((outp[offset_fat32 + 21] >> 0) & 0xFF);
    fat32->RootDirectoryStart[1] = ((outp[offset_fat32 + 22] >> 8) & 0xFF);
    fat32->RootDirectoryStart[2] = ((outp[offset_fat32 + 22] >> 0) & 0xFF);
    fat32->RootDirectoryStart[3] = ((outp[offset_fat32 + 23] >> 8) & 0xFF);

    fat32->FSInfoSector[0] = ((outp[offset_fat32 + 23] >> 0) & 0xFF);
    fat32->FSInfoSector[1] = ((outp[offset_fat32 + 24] >> 8) & 0xFF);

    fat32->BackupBootSector[0] = ((outp[offset_fat32 + 24] >> 0) & 0xFF);
    fat32->BackupBootSector[1] = ((outp[offset_fat32 + 25] >> 8) & 0xFF);

    return memory_addr;
}

void GetListOfFiles(FAT_Folder_t *folderstruct, uint64_t *amount)
{
    uint64_t short_unique_ident = 0x10300000000;
    uint64_t short_addr = request_memory(200, &short_unique_ident);
    FAT32_t* fat32 = (FAT32_t*)ReadParameter();
    uint32_t lba = 
                    fat32->RootDirectoryStart[0] << 24 |
                    fat32->RootDirectoryStart[1] << 16 |
                    fat32->RootDirectoryStart[2] <<  8 |
                    fat32->RootDirectoryStart[3] <<  0;
    ReadSectorsLBA(0, lba, 1, &short_addr);
    for (uint8_t i = 0; i < 200; i++)
    {
        uint8_t* values_addr = (uint8_t*)short_addr;
        uint16_t byte_read = values_addr[i * 20 + 0];
        if (byte_read == 0)
        {
            i = 200;
            continue;
        }
        folderstruct[i].NAME[0] = byte_read;
        folderstruct[i].NAME[1] = values_addr[i * 20 + 1];
        folderstruct[i].NAME[2] = values_addr[i * 0x20 + 2];
        folderstruct[i].NAME[3] = values_addr[i * 0x20 + 3];
        folderstruct[i].NAME[4] = values_addr[i * 0x20 + 4];
        folderstruct[i].NAME[5] = values_addr[i * 0x20 + 5];
        folderstruct[i].NAME[6] = values_addr[i * 0x20 + 6];
        folderstruct[i].NAME[7] = values_addr[i * 0x20 + 7];
        folderstruct[i].NAME[8] = values_addr[i * 0x20 + 8];
        folderstruct[i].NAME[9] = values_addr[i * 0x20 + 9];
        folderstruct[i].NAME[10] = values_addr[i * 0x20 + 10];
        *amount = i;
    }
    unblock_memory(&short_unique_ident);
}

void ReadSectorsLBA(uint8_t drive_num, uint32_t start_lba, uint8_t sector_count, uint64_t *dest)
{
    uint16_t index, max, base;
    uint64_t *buffer = (uint64_t *)dest;
    uint8_t drive = 0x40;

    switch (drive_num)
    {
        case 0:
        case 1:
            base = 0x1F0; break;
        case 2:
        case 3:
            base = 0x170; break;
        case 4:
        case 5:
            base = 0x1E8; break;
        case 6:
        case 7:
            base = 0x168; break;
        default:
            return;
    }
    if (drive_num % 2)
    {
        drive |= 0x10;
    }
    outb(base + 2, sector_count);
    outb(base + 3, (uint8_t)((start_lba >> 0)  & 0xFF));
    outb(base + 4, (uint8_t)((start_lba >> 8)  & 0xFF));
    outb(base + 5, (uint8_t)((start_lba >> 16) & 0xFF));
    outb(base + 6, (uint8_t)((start_lba >> 24) & 0x0F) | drive);
    outb(base + 7, 0x20);

    while ((uint8_t)inb(base + 7) & 0x80) ;
    
    max = (sector_count * (512 / 2));
    for (index = 0; index < max; index++)
    {
        uint16_t value = inw(base + 0);
        buffer[index] = (((value & 0xFF) << 8) | ((value >> 8) & 0xFF));
    }
}

uint8_t lastIndex(char *tmp, uint8_t max)
{
    uint8_t index = 0;
    while (*tmp)
    {
        index++;
        tmp++;
        if (index == max)
        {
            return index;
        }
    }
    return index;
}
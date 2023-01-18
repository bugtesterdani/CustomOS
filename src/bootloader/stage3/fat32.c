#include "headers/fat32.h"
#include "headers/memory_management.h"

#include "headers/stdio.h"
#include "headers/isr.h"
#include "headers/io.h"

#include "headers/colors.h"
#include "headers/screen.h"
#include "headers/string.h"
#include "headers/stdio.h"
#include "headers/commands.h"

static FAT32_t *fat32;

void FAT32_setup_static(uint32_t address_fat32_informations)
{
    fat32 = (FAT32_t*)address_fat32_informations;
}

void ReadParameter(FAT32_t **address_fat32, uint8_t drive_num)
{
    uint8_t sector_amount = 1;

    uint32_t address = 0;
    uint32_t offset = 0;
    if (allocate_block(&address, &offset, 512 * sector_amount) == 0)
    {
        // Failed to allocate space
        stopping_system();
        return;
    }
    
    // outp[256 * sector_amount]
    uint16_t *outp = (uint16_t*)(address + offset);
    ReadSectorsLBA(drive_num, 0, sector_amount, outp, 1);

    // 1 => FAT32 Offset. First 3 Bytes are not useful for the informations
    fat32->OEM_ID[0] = ((outp[1 + 0] >> 0) & 0xFF);
    fat32->OEM_ID[1] = ((outp[1 + 1] >> 8) & 0xFF);
    fat32->OEM_ID[2] = ((outp[1 + 1] >> 0) & 0xFF);
    fat32->OEM_ID[3] = ((outp[1 + 2] >> 8) & 0xFF);
    fat32->OEM_ID[4] = ((outp[1 + 2] >> 0) & 0xFF);
    fat32->OEM_ID[5] = ((outp[1 + 3] >> 8) & 0xFF);
    fat32->OEM_ID[6] = ((outp[1 + 3] >> 0) & 0xFF);
    fat32->OEM_ID[7] = ((outp[1 + 4] >> 8) & 0xFF);

    fat32->BytesPerSector[0] = ((outp[1 + 4] >> 0) & 0xFF);
    fat32->BytesPerSector[1] = ((outp[1 + 5] >> 8) & 0xFF);

    fat32->SectorsPerCluster = ((outp[1 + 5] >> 0) & 0xFF);

    fat32->ReservedSectors[0] = ((outp[1 + 6] >> 8) & 0xFF);
    fat32->ReservedSectors[1] = ((outp[1 + 6] >> 0) & 0xFF);

    fat32->TotalFATs[0] = ((outp[1 + 7] >> 8) & 0xFF);

    fat32->MaxRootEntries[0] = ((outp[1 + 7] >> 0) & 0xFF);
    fat32->MaxRootEntries[1] = ((outp[1 + 8] >> 8) & 0xFF);

    fat32->NumberOfSectors[0] = ((outp[1 + 8] >> 0) & 0xFF);
    fat32->NumberOfSectors[1] = ((outp[1 + 9] >> 8) & 0xFF);

    fat32->MediaDescriptor = ((outp[1 + 9] >> 0) & 0xFF);

    fat32->SectorsPerFAT[0] = ((outp[1 + 10] >> 8) & 0xFF);
    fat32->SectorsPerFAT[1] = ((outp[1 + 10] >> 0) & 0xFF);

    fat32->SectorsPerTrack[0] = ((outp[1 + 11] >> 8) & 0xFF);
    fat32->SectorsPerTrack[1] = ((outp[1 + 11] >> 0) & 0xFF);

    fat32->SectorsPerHead[0] = ((outp[1 + 12] >> 8) & 0xFF);
    fat32->SectorsPerHead[1] = ((outp[1 + 12] >> 0) & 0xFF);

    fat32->HiddenSectors[0] = ((outp[1 + 13] >> 8) & 0xFF);
    fat32->HiddenSectors[1] = ((outp[1 + 13] >> 0) & 0xFF);
    fat32->HiddenSectors[2] = ((outp[1 + 14] >> 8) & 0xFF);
    fat32->HiddenSectors[3] = ((outp[1 + 14] >> 0) & 0xFF);

    fat32->TotalSectors[0] = ((outp[1 + 15] >> 8) & 0xFF);
    fat32->TotalSectors[1] = ((outp[1 + 15] >> 0) & 0xFF);
    fat32->TotalSectors[2] = ((outp[1 + 16] >> 8) & 0xFF);
    fat32->TotalSectors[3] = ((outp[1 + 16] >> 0) & 0xFF);

    fat32->BigSectorsPerFAT[0] = ((outp[1 + 17] >> 8) & 0xFF);
    fat32->BigSectorsPerFAT[1] = ((outp[1 + 17] >> 0) & 0xFF);
    fat32->BigSectorsPerFAT[2] = ((outp[1 + 18] >> 8) & 0xFF);
    fat32->BigSectorsPerFAT[3] = ((outp[1 + 18] >> 0) & 0xFF);

    fat32->Flags[0] = ((outp[1 + 19] >> 8) & 0xFF);
    fat32->Flags[1] = ((outp[1 + 19] >> 0) & 0xFF);

    fat32->FSVersion[0] = ((outp[1 + 20] >> 8) & 0xFF);
    fat32->FSVersion[1] = ((outp[1 + 20] >> 0) & 0xFF);

    fat32->RootDirectoryStart[0] = ((outp[1 + 21] >> 8) & 0xFF);
    fat32->RootDirectoryStart[1] = ((outp[1 + 21] >> 0) & 0xFF);
    fat32->RootDirectoryStart[2] = ((outp[1 + 22] >> 8) & 0xFF);
    fat32->RootDirectoryStart[3] = ((outp[1 + 22] >> 0) & 0xFF);

    fat32->FSInfoSector[0] = ((outp[1 + 23] >> 8) & 0xFF);
    fat32->FSInfoSector[1] = ((outp[1 + 23] >> 0) & 0xFF);

    fat32->BackupBootSector[0] = ((outp[1 + 24] >> 8) & 0xFF);
    fat32->BackupBootSector[1] = ((outp[1 + 24] >> 0) & 0xFF);

    *address_fat32 = fat32;

    // Freeup the memoryspace again
    unblock_space(&address, 512 * sector_amount);
}

void GetListOfFiles(uint8_t drive_num, FAT_Folder_t *folderstruct, uint32_t *amount)
{
    if (fat32->OEM_ID[0] == 0)
    {
        FAT32_t *fat32;
        ReadParameter(&fat32, drive_num);
    }
    uint32_t FirstDataSector =  (*((uint16_t*)fat32->ReservedSectors)) + 
                                ((*((uint8_t*)fat32->TotalFATs)) * (*((uint32_t*)fat32->BigSectorsPerFAT))) +
                                (*((uint16_t*)fat32->MaxRootEntries));
    uint32_t FirstSectorOfCluster = (((*((uint32_t*)fat32->RootDirectoryStart)) - 2) * fat32->SectorsPerCluster) + 
                                    FirstDataSector;
    
    uint32_t address = 0;
    uint32_t offset = 0;
    if (allocate_block(&address, &offset, 512 * 8) == 0)
    {
        // Failed to allocate space
        stopping_system();
        return;
    }

    uint16_t *short_addr = (uint16_t*)(address + offset);
    for (uint16_t k = 0; k < 0x10; k++)
    {
        clearArray(((uint8_t*)(address + offset)), 512, 0x00);
        ReadSectorsLBA(drive_num, FirstSectorOfCluster + k, 1, short_addr, 0);
        uint8_t count = 0;
        for (uint8_t i = 0; i < 0x20; i++)
        {
            uint16_t *values = ((uint16_t*)&(short_addr[i * 0x10]));
            count += 1;
            if (((values[0] >> 8) & 0xFF) == 0)
            {
                i = 0x20;
            }
        }

        if (count <= 1)
        {
            break;
        }

        FAT_Folder_t *values = ((FAT_Folder_t*)(address + offset));
        for (uint16_t i = 0; i < count; i++)
        {
            folderstruct[i] = values[i];
        }

        amount[0] += count - 1;

        if (count < 0x21)
        {
            break;
        }
    }
    
    // Freeup the memoryspace again
    unblock_space(&address, 512 * 8);
}

void ReadFile(uint8_t drive_num, uint32_t offset_index, uint64_t byte_size, uint16_t *data_address)
{
    if (fat32->OEM_ID[0] == 0)
    {
        FAT32_t *fat32;
        ReadParameter(&fat32, drive_num);
    }

    uint32_t FirstDataSector =  (*((uint16_t*)fat32->ReservedSectors)) + 
                                ((*((uint8_t*)fat32->TotalFATs)) * (*((uint32_t*)fat32->BigSectorsPerFAT))) +
                                (*((uint16_t*)fat32->MaxRootEntries));
    uint32_t FirstSectorOfCluster = (((*((uint32_t*)fat32->RootDirectoryStart)) - 2) * fat32->SectorsPerCluster) + 
                                    FirstDataSector;

    uint8_t sector_count = (byte_size / 512);
    if ((byte_size % 512) > 0)
    {
        sector_count++;
    }

    ReadSectorsLBA(drive_num, FirstSectorOfCluster + ((offset_index - 2) * fat32->SectorsPerCluster), sector_count, data_address, 0);
}

void ReadSectorsLBA(uint8_t drive_num, uint32_t start_lba, uint8_t sector_count, uint16_t *dest, uint8_t reverted)
{
    uint16_t index, max, base;
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
        if (reverted)
        {
            dest[index] = (((value & 0xFF) << 8) | ((value >> 8) & 0xFF));
        }
        else
        {
            dest[index] = value;
        }
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
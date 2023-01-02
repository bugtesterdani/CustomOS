#pragma once

#include "stdint.h"

typedef struct FAT32
{
    uint8_t OEM_ID[8];
    uint8_t BytesPerSector[2];
    uint8_t SectorsPerCluster;
    uint8_t ReservedSectors[2];
    uint8_t TotalFATs[2];
    uint8_t MaxRootEntries[2];
    uint8_t NumberOfSectors[2];
    uint8_t MediaDescriptor;
    uint8_t SectorsPerFAT[2];
    uint8_t SectorsPerTrack[2];
    uint8_t SectorsPerHead[2];
    uint8_t HiddenSectors[4];
    uint8_t TotalSectors[4];
    uint8_t BigSectorsPerFAT[4];
    uint8_t Flags[2];
    uint8_t FSVersion[2];
    uint8_t RootDirectoryStart[4];
    uint8_t FSInfoSector[2];
    uint8_t BackupBootSector[2];
} __attribute__((packed)) FAT32_t;

typedef struct FAT_Folder
{
    uint8_t NAME[11];
} __attribute__((packed)) FAT_Folder_t;

uint64_t ReadParameter();
void GetListOfFiles(FAT_Folder_t *folderstruct, uint64_t *amount);
void ReadSectorsLBA(uint8_t drive_num, uint32_t start_lba, uint8_t sector_count, uint64_t *dest);
uint8_t lastIndex(char *tmp, uint8_t max);

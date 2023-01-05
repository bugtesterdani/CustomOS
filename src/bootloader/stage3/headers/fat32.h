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
    uint8_t Attribute;
    uint8_t Reserved[2];
    // Hour   (Bits 15-11) => (0 - 23)
    // Minute (Bits 10- 5) => (0 - 59)
    // Second (Bits  4- 0) => (0 - 29)
    uint8_t CreateTime[2];
    // Year   (Bits 15- 9) => (1980 - 2107)
    // Month  (Bits  8- 5) => (1 - 12)
    // Day    (Bits  4- 0) => (1 - 31)
    uint8_t CreateDate[2];
    // Year   (Bits 15- 9) => (1980 - 2107)
    // Month  (Bits  8- 5) => (1 - 12)
    // Day    (Bits  4- 0) => (1 - 31)
    uint8_t LastAccessDate[2];
    uint8_t HighBytes_Cluster[2];
    // Year   (Bits 15- 9) => (1980 - 2107)
    // Month  (Bits  8- 5) => (1 - 12)
    // Day    (Bits  4- 0) => (1 - 31)
    uint8_t LastModifiedTime[2];
    // Year   (Bits 15- 9) => (1980 - 2107)
    // Month  (Bits  8- 5) => (1 - 12)
    // Day    (Bits  4- 0) => (1 - 31)
    uint8_t LastModifiedDate[2];
    uint8_t LowBytes_Cluster[2];
    uint8_t FileSize_Byte[4];
} __attribute__((packed)) FAT_Folder_t;

enum FAT_Folder_Attribute_MASK
{
    ReadOnly        = 0x01,
    Hidden          = 0x02,
    System          = 0x04,
    VolumeLabel     = 0x08,
    Subdirectory    = 0x10,
    Archive         = 0x20,
    Device          = 0x40,
    Reserved        = 0x80
};

uint64_t ReadParameter();
void GetListOfFiles(FAT_Folder_t *folderstruct, uint32_t *amount);
void ReadSectorsLBA(uint8_t drive_num, uint32_t start_lba, uint8_t sector_count, uint16_t *dest);
uint8_t lastIndex(char *tmp, uint8_t max);

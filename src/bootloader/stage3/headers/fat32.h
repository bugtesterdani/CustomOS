#pragma once

#include "stdint.h"

struct FAT32
{
    uint64_t OEM_ID;
    uint16_t BytesPerSector;
    uint8_t SectorsPerCluster;
    uint16_t ReservedSectors;
    uint16_t TotalFATs;
    uint16_t MaxRootEntries;
    uint16_t NumberOfSectors;
    uint8_t MediaDescriptor;
    uint16_t SectorsPerFAT;
    uint16_t SectorsPerTrack;
    uint16_t SectorsPerHead;
    uint32_t HiddenSectors;
    uint32_t TotalSectors;
    uint32_t BigSectorsPerFAT;
    uint16_t Flags;
    uint16_t FSVersion;
    uint32_t RootDirectoryStart;
    uint16_t FSInfoSector;
    uint16_t BackupBootSector;
} __attribute__((packed));

typedef struct FAT32 FAT32_t;

void ReadAddress(uint8_t *buffer, unsigned long int address, unsigned long int amount_chars);
void ReadParameter(FAT32_t *fat32);

#pragma once

#include "stdint.h"
#include "pci.h"

// Channels:
#define      ATA_PRIMARY      0x00
#define      ATA_SECONDARY    0x01

// ATA-Commands:
#define      ATA_CMD_READ_PIO         0x20
#define      ATA_CMD_READ_PIO_EXT     0x24
#define      ATA_CMD_READ_DMA         0xC8
#define      ATA_CMD_READ_DMA_EXT     0x25
#define      ATA_CMD_WRITE_PIO        0x30
#define      ATA_CMD_WRITE_PIO_EXT    0x34
#define      ATA_CMD_WRITE_DMA        0xCA
#define      ATA_CMD_WRITE_DMA_EXT    0x35
#define      ATA_CMD_CACHE_FLUSH      0xE7
#define      ATA_CMD_CACHE_FLUSH_EXT  0xEA
#define      ATA_CMD_PACKET           0xA0
#define      ATA_CMD_IDENTIFY_PACKET  0xA1
#define      ATA_CMD_IDENTIFY         0xEC

#define      ATA_MASTER_PORT          0xA0
#define      ATA_SLAVE_PORT_BIT       0x01

struct ATA
{
    uint16_t port, slavebit;
    uint32_t LBA28;
    uint64_t LBA48;
    uint8_t isLBA48Supported;
} __attribute__((packed));

typedef struct ATA ATA_t;

void ATA_Init(ATA_t loc_ATADevices[4], uint8_t *loc_count);
void ATA_Detect_Devtype(uint16_t port, uint8_t slavebit);
void ataRead(uint8_t drive_id, uint64_t LBA, uint8_t cnt, void *addr);
void ataWrite(uint8_t drive_id, uint64_t LBA, uint8_t cnt, void *addr);
void ataRead28(uint8_t drive_id, uint64_t LBA, uint8_t cnt, void *addr);
void ataWrite28(uint8_t drive_id, uint64_t LBA, uint8_t cnt, void *addr);
void ataWrite48(uint8_t drive_id, uint64_t LBA, uint8_t cnt, void *addr);
void ataRead48(uint8_t drive_id, uint64_t LBA, uint8_t cnt, void *addr);
void ata_detect_devices();

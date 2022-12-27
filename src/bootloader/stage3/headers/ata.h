#pragma once

#include "stdint.h"
#include "pci.h"

#define      IDE_ATA         0x00
#define      IDE_ATAPI       0x01

#define      ATA_MASTER      0x00
#define      ATA_SLAVE       0x01

// Channels:
#define      ATA_PRIMARY      0x00
#define      ATA_SECONDARY    0x01

// Directions:
#define      ATA_READ       0x00
#define      ATA_WRITE      0x01

#define    ATA_SR_BSY       0x80
#define    ATA_SR_DRDY      0x40
#define    ATA_SR_DF        0x20
#define    ATA_SR_DSC       0x10
#define    ATA_SR_DRQ       0x08
#define    ATA_SR_CORR      0x04
#define    ATA_SR_IDX       0x02
#define    ATA_SR_ERR       0x01

#define    ATA_ER_BBK      0x80
#define    ATA_ER_UNC      0x40
#define    ATA_ER_MC       0x20
#define    ATA_ER_IDNF     0x10
#define    ATA_ER_MCR      0x08
#define    ATA_ER_ABRT     0x04
#define    ATA_ER_TK0NF    0x02
#define    ATA_ER_AMNF     0x01

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

/*ATAPI - We will most likely not implement*/
#define      ATAPI_CMD_READ      0xA8
#define      ATAPI_CMD_EJECT      0x1B

/*The Commands ATA_CMD_IDENTIFY_PACKET, and ATA_CMD_IDENTIFY, returns a buffer of 512 byte,*/
/*the buffer is called Identification space, the following definitions are used to read information from the identification space.*/
#define    ATA_IDENT_DEVICETYPE   0
#define    ATA_IDENT_CYLINDERS   2
#define    ATA_IDENT_HEADS      6
#define    ATA_IDENT_SECTORS      12
#define    ATA_IDENT_SERIAL   20
#define    ATA_IDENT_MODEL      54
#define    ATA_IDENT_CAPABILITIES   98
#define    ATA_IDENT_FIELDVALID   106
#define    ATA_IDENT_MAX_LBA   120
#define    ATA_IDENT_COMMANDSETS   164
#define    ATA_IDENT_MAX_LBA_EXT   200

// ATA-ATAPI Task-File:
#define      ATA_REG_DATA      0x00
#define      ATA_REG_ERROR      0x01
#define      ATA_REG_FEATURES   0x01
#define      ATA_REG_SECCOUNT0   0x02
#define      ATA_REG_LBA0      0x03
#define      ATA_REG_LBA1      0x04
#define      ATA_REG_LBA2      0x05
#define      ATA_REG_HDDEVSEL   0x06
#define      ATA_REG_COMMAND      0x07
#define      ATA_REG_STATUS      0x07
#define      ATA_REG_SECCOUNT1   0x08
#define      ATA_REG_LBA3      0x09
#define      ATA_REG_LBA4      0x0A
#define      ATA_REG_LBA5      0x0B
#define      ATA_REG_CONTROL      0x0C
#define      ATA_REG_ALTSTATUS   0x0C
#define      ATA_REG_DEVADDRESS   0x0D

struct channel
{
    uint16_t base;
    uint16_t ctrl;
    uint16_t bmide;
    uint8_t nIEN;
} __attribute__((packed));

struct ide_device
{
    uint8_t reseerved, channel, drive;
    uint16_t type, sign, capabilities;
    uint32_t commandsets, size;
    uint8_t model[4];
} __attribute__((packed));

typedef struct channel channel_t;
typedef struct ide_device ide_device_t;

void InitializeATA(channel_t channel[2], ide_device_t device[4], uint8_t ide_buff[2048]);
void ata_GetIDE();
void ata_GetSATA();
void ata_checkDevice(pci_dev_t *dev);
void ata_ide_initialize(pci_dev_t *dev, pci_baddress_t *baddress);
void ata_parseIDEDevice(uint8_t i, uint8_t j, uint8_t imax);
void ata_ide_write(uint8_t channel, uint8_t reg, uint8_t data);
uint8_t ata_ide_read(uint8_t channel, uint8_t reg);
void ata_ide_read_buffer(uint8_t channel, uint8_t reg, uint32_t buffer, uint32_t quads);
void ata_400nsdelay();
void ata_cache_flush_write(uint8_t channel, uint8_t reg);

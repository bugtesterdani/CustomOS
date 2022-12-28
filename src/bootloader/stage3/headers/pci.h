#pragma once

#include "stdint.h"

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC
#define PCI_DEV_COUNT 20

struct pci_dev
{
    uint8_t     bus, device, function;
    uint16_t    vendorID, deviceID,
                Command, Status;
    uint8_t     RevisionID, ProgIF, Subclass, ClassCode,
                CacheLineSize, LatencyTimer, HeaderType, BIST;
} __attribute__((packed));

struct pci_baddress
{
    uint32_t BaseAddress0, BaseAddress1, BaseAddress2, BaseAddress3, BaseAddress4, BaseAddress5;
} __attribute__((packed));

typedef struct pci_dev pci_dev_t;
typedef struct pci_baddress pci_baddress_t;

uint16_t pci_ConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint32_t pci_ConfigReadDword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
void pci_ConfigWriteDword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t value);
uint32_t pci_MakeAddress(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
void pci_getBaseAddress(uint8_t bus, uint8_t slot, uint8_t function, pci_baddress_t *address);
uint16_t pci_getVendorId(uint8_t bus, uint8_t slot, uint8_t function);
uint16_t pci_getDeviceId(uint8_t bus, uint8_t slot, uint8_t function);
uint16_t pci_getCommand(uint8_t bus, uint8_t slot, uint8_t function);
uint16_t pci_getStatus(uint8_t bus, uint8_t slot, uint8_t function);
uint8_t pci_getRevisionID(uint8_t bus, uint8_t slot, uint8_t function);
uint8_t pci_getProgIf(uint8_t bus, uint8_t slot, uint8_t function);
uint8_t pci_getSubClass(uint8_t bus, uint8_t slot, uint8_t function);
uint8_t pci_getBaseClass(uint8_t bus, uint8_t slot, uint8_t function);
uint8_t pci_getCacheLineSize(uint8_t bus, uint8_t slot, uint8_t function);
uint8_t pci_getLatencyTimer(uint8_t bus, uint8_t slot, uint8_t function);
uint8_t pci_getHeaderType(uint8_t bus, uint8_t slot, uint8_t function);
uint8_t pci_getBIST(uint8_t bus, uint8_t slot, uint8_t function);
void pci_checkDevice(uint8_t bus, uint8_t device);
void pci_checkBus(uint8_t bus);
void pci_checkFunction(uint8_t bus, uint8_t device, uint8_t function);
void pci_checkAllBuses(void);
void pci_init(pci_dev_t devs[PCI_DEV_COUNT]);

#pragma once

#include "stdint.h"

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC

static uint8_t test = 0;

typedef struct
{
    uint8_t bus, device, function;
    uint16_t vendorID, deviceID;
} pci_dev;

uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

uint32_t pciConfigReadDword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
void pciConfigWriteDword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t value);

uint32_t pciMakeAddress(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

uint16_t getVendorId(uint8_t bus, uint8_t slot, uint8_t function);

uint16_t getDeviceId(uint8_t bus, uint8_t slot, uint8_t function);

uint16_t getCommand(uint8_t bus, uint8_t slot, uint8_t function);

uint16_t getStatus(uint8_t bus, uint8_t slot, uint8_t function);

uint8_t getRevisionID(uint8_t bus, uint8_t slot, uint8_t function);

uint8_t getProgIf(uint8_t bus, uint8_t slot, uint8_t function);

uint8_t getSubClass(uint8_t bus, uint8_t slot, uint8_t function);

uint8_t getBaseClass(uint8_t bus, uint8_t slot, uint8_t function);

uint8_t getCacheLineSize(uint8_t bus, uint8_t slot, uint8_t function);

uint8_t getLatencyTimer(uint8_t bus, uint8_t slot, uint8_t function);
uint8_t getHeaderType(uint8_t bus, uint8_t slot, uint8_t function);
uint8_t getBIST(uint8_t bus, uint8_t slot, uint8_t function);
void checkDevice(uint8_t bus, uint8_t device);
void checkBus(uint8_t bus);
void checkFunction(uint8_t bus, uint8_t device, uint8_t function);
void checkAllBuses(void);
void pci_init(pci_dev *devs);

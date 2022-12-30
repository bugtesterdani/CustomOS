#include "headers/stdint.h"
#include "headers/pci.h"
#include "headers/io.h"

pci_dev_t pcidevs[PCI_DEV_COUNT];
uint16_t numdevs=0;

uint16_t pci_ConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t value = pci_ConfigReadDword(bus, slot, func, offset);
    return ((value >> ((offset & 2) * 8)) & 0xFFFF);
}

uint32_t pci_ConfigReadDword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address = pci_MakeAddress(bus, slot, func, offset);
    outl(CONFIG_ADDRESS, address);
    return inl(CONFIG_DATA);
}

void pci_ConfigWriteDword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t value)
{
    uint32_t address = pci_MakeAddress(bus, slot, func, offset);
    outl(CONFIG_ADDRESS, address);
    outl(CONFIG_DATA, value);
}

uint32_t pci_MakeAddress(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    return (uint32_t)
            (
                (bus << 16) |
                (slot << 11) |
                (func << 8) |
                (offset & 0xFC) |
                ((uint32_t)0x80000000));
}

uint16_t pci_getVendorId(uint8_t bus, uint8_t slot, uint8_t function)
{
	return pci_ConfigReadWord(bus,slot,function,0x00);
}

void pci_getBaseAddress(uint8_t bus, uint8_t slot, uint8_t function, pci_baddress_t *address)
{
	(*address).BaseAddress0 = pci_ConfigReadDword(bus, slot, function, 0x10);
	(*address).BaseAddress1 = pci_ConfigReadDword(bus, slot, function, 0x14);
	(*address).BaseAddress2 = pci_ConfigReadDword(bus, slot, function, 0x18);
	(*address).BaseAddress3 = pci_ConfigReadDword(bus, slot, function, 0x1C);
	(*address).BaseAddress4 = pci_ConfigReadDword(bus, slot, function, 0x20);
	(*address).BaseAddress5 = pci_ConfigReadDword(bus, slot, function, 0x24);
}

uint16_t pci_getDeviceId(uint8_t bus, uint8_t slot, uint8_t function)
{
	return pci_ConfigReadWord(bus,slot,function,0x02);
}

uint16_t pci_getCommand(uint8_t bus, uint8_t slot, uint8_t function)
{
    return pci_ConfigReadWord(bus,slot,function,0x04);
}

uint16_t pci_getStatus(uint8_t bus, uint8_t slot, uint8_t function)
{
    return pci_ConfigReadWord(bus,slot,function,0x06);
}

uint8_t pci_getRevisionID(uint8_t bus, uint8_t slot, uint8_t function)
{
    return (uint8_t)(pci_ConfigReadWord(bus,slot,function,0x08) & 0xFF);
}

uint8_t pci_getProgIf(uint8_t bus, uint8_t slot, uint8_t function)
{
	return (uint8_t)((pci_ConfigReadWord(bus,slot,function,0x08) >> 8) & 0xFF);
}

uint8_t pci_getSubClass(uint8_t bus, uint8_t slot, uint8_t function)
{
	return (uint8_t)(pci_ConfigReadWord(bus,slot,function,0x0A) & 0xFF);
}

uint8_t pci_getBaseClass(uint8_t bus, uint8_t slot, uint8_t function)
{
	return (uint8_t)((pci_ConfigReadWord(bus,slot,function,0x0A) >> 8) & 0xFF);
}

uint8_t pci_getCacheLineSize(uint8_t bus, uint8_t slot, uint8_t function)
{
    return (uint8_t)(pci_ConfigReadWord(bus,slot,function,0x0C) & 0xFF);
}

uint8_t pci_getLatencyTimer(uint8_t bus, uint8_t slot, uint8_t function)
{
    return (uint8_t)((pci_ConfigReadWord(bus,slot,function,0x0C) >> 8) & 0xFF);
}

uint8_t pci_getHeaderType(uint8_t bus, uint8_t slot, uint8_t function)
{
	return (uint8_t)(pci_ConfigReadWord(bus,slot,function,0x0E) & 0xFF);
}

uint8_t pci_getBIST(uint8_t bus, uint8_t slot, uint8_t function)
{
    return (uint8_t)((pci_ConfigReadWord(bus,slot,function,0x0E) >> 8) & 0xFF);
}

uint8_t pci_getSecondaryBus(uint8_t bus, uint8_t slot, uint8_t function)
{
	return pci_ConfigReadWord(bus,slot,function,0x18) >> 8;
}

void pci_checkDevice(uint8_t bus, uint8_t device)
{
	uint8_t function = 0;
	uint16_t vendorId = pci_getVendorId(bus,device,function);
	if(vendorId==0xFFFF) return;
	pci_checkFunction(bus,device,function);
	uint16_t headerType = pci_getHeaderType(bus,device,function);
	if (headerType & 0x80) {
	  for(function=1;function<8;++function) {
	    if(pci_getVendorId(bus,device,function) != 0xFFFF)
	       pci_checkFunction(bus,device,function);
	  }
	}
}

void pci_checkBus(uint8_t bus)
{
	uint8_t device;
	for(device=0;device<32;++device)
		pci_checkDevice(bus,device);
}

void pci_checkFunction(uint8_t bus, uint8_t device, uint8_t function)
{
	uint8_t revisionid, progif, subclass, classcode,
			cachelinesize, latencytimer, headertype, bist;
	revisionid = pci_getRevisionID(bus, device, function);
	progif = pci_getProgIf(bus, device, function);
	subclass = pci_getSubClass(bus,device,function);
	classcode = pci_getBaseClass(bus,device,function);
	cachelinesize = pci_getCacheLineSize(bus, device, function);
	latencytimer = pci_getLatencyTimer(bus, device, function);
	headertype = pci_getHeaderType(bus, device, function);
	bist = pci_getBIST(bus, device, function);
	uint16_t vendorId, deviceId, command, status;
	vendorId = pci_getVendorId(bus,device,function);
	deviceId = pci_getDeviceId(bus,device,function);
	command = pci_getCommand(bus,device,function);
	status = pci_getStatus(bus,device,function);
	pcidevs[numdevs].bus=bus;
	pcidevs[numdevs].device=device;
	pcidevs[numdevs].function=function;
	pcidevs[numdevs].vendorID=vendorId;
	pcidevs[numdevs].deviceID=deviceId;
	pcidevs[numdevs].Command=command;
	pcidevs[numdevs].Status=status;
	pcidevs[numdevs].ProgIF=progif;
	pcidevs[numdevs].Subclass=subclass;
	pcidevs[numdevs].ClassCode=classcode;
	pcidevs[numdevs].CacheLineSize=cachelinesize;
	pcidevs[numdevs].LatencyTimer=latencytimer;
	pcidevs[numdevs].HeaderType=headertype;
	pcidevs[numdevs].BIST=bist;
	++numdevs;
	//printFunction(bus,device,function);
	if((classcode==0x06) && (subclass==0x04)) {
		uint8_t secondaryBus = pci_getSecondaryBus(bus,device,function);
		pci_checkBus(secondaryBus);
	}
}

void pci_checkAllBuses(void)
{
	uint8_t function;
	if((pci_getHeaderType(0,0,0)&0x80)==0) {
		pci_checkBus(0);
	} else {
		for(function=0;function>8;++function) {
			if(pci_getVendorId(0,0,function)!=0xFFFF) break;
			pci_checkBus(function);
		}
	}
}

void pci_init(pci_dev_t devs[PCI_DEV_COUNT])
{
	numdevs = 0;
    pci_checkAllBuses();
	for (uint8_t i = 0; i < PCI_DEV_COUNT; i++)
	{
		devs[i] = pcidevs[i];
	}
    // devs = pcidevs;
}

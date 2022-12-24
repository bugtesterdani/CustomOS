#include "headers/stdint.h"
#include "headers/pci.h"
#include "headers/io.h"

pci_dev pcidevs[PCI_DEV_COUNT];
uint16_t numdevs=0;

uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t value = pciConfigReadDword(bus, slot, func, offset);
    return ((value >> ((offset & 2) * 8)) & 0xFFFF);
}

uint32_t pciConfigReadDword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address = pciMakeAddress(bus, slot, func, offset);
    outl(CONFIG_ADDRESS, address);
    return inl(CONFIG_DATA);
}

void pciConfigWriteDword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t value)
{
    uint32_t address = pciMakeAddress(bus, slot, func, offset);
    outl(CONFIG_ADDRESS, address);
    outl(CONFIG_DATA, value);
}

uint32_t pciMakeAddress(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    return (uint32_t)
            (
                (bus << 16) |
                (slot << 11) |
                (func << 8) |
                (offset & 0xFC) |
                ((uint32_t)0x80000000));
}

uint16_t getVendorId(uint8_t bus, uint8_t slot, uint8_t function)
{
	return pciConfigReadWord(bus,slot,function,0x00);
}

uint16_t getDeviceId(uint8_t bus, uint8_t slot, uint8_t function)
{
	return pciConfigReadWord(bus,slot,function,0x02);
}

uint16_t getCommand(uint8_t bus, uint8_t slot, uint8_t function)
{
    return pciConfigReadWord(bus,slot,function,0x04);
}

uint16_t getStatus(uint8_t bus, uint8_t slot, uint8_t function)
{
    return pciConfigReadWord(bus,slot,function,0x06);
}

uint8_t getRevisionID(uint8_t bus, uint8_t slot, uint8_t function)
{
    return (uint8_t)(pciConfigReadWord(bus,slot,function,0x08) & 0xFF);
}

uint8_t getProgIf(uint8_t bus, uint8_t slot, uint8_t function)
{
	return (uint8_t)((pciConfigReadWord(bus,slot,function,0x08) >> 8) & 0xFF);
}

uint8_t getSubClass(uint8_t bus, uint8_t slot, uint8_t function)
{
	return (uint8_t)(pciConfigReadWord(bus,slot,function,0x0A) & 0xFF);
}

uint8_t getBaseClass(uint8_t bus, uint8_t slot, uint8_t function)
{
	return (uint8_t)((pciConfigReadWord(bus,slot,function,0x0A) >> 8) & 0xFF);
}

uint8_t getCacheLineSize(uint8_t bus, uint8_t slot, uint8_t function)
{
    return (uint8_t)(pciConfigReadWord(bus,slot,function,0x0C) & 0xFF);
}

uint8_t getLatencyTimer(uint8_t bus, uint8_t slot, uint8_t function)
{
    return (uint8_t)((pciConfigReadWord(bus,slot,function,0x0C) >> 8) & 0xFF);
}

uint8_t getHeaderType(uint8_t bus, uint8_t slot, uint8_t function)
{
	return (uint8_t)(pciConfigReadWord(bus,slot,function,0x0E) & 0xFF);
}

uint8_t getBIST(uint8_t bus, uint8_t slot, uint8_t function)
{
    return (uint8_t)((pciConfigReadWord(bus,slot,function,0x0E) >> 8) & 0xFF);
}

uint8_t getSecondaryBus(uint8_t bus, uint8_t slot, uint8_t function)
{
	return pciConfigReadWord(bus,slot,function,0x18) >> 8;
}

void checkDevice(uint8_t bus, uint8_t device)
{
	uint8_t function = 0;
	uint16_t vendorId = getVendorId(bus,device,function);
	if(vendorId==0xFFFF) return;
	checkFunction(bus,device,function);
	uint16_t headerType = getHeaderType(bus,device,function);
	if (headerType & 0x80) {
	  for(function=1;function<8;++function) {
	    if(getVendorId(bus,device,function) != 0xFFFF)
	       checkFunction(bus,device,function);
	  }
	}
}

void checkBus(uint8_t bus)
{
	uint8_t device;
	for(device=0;device<32;++device)
		checkDevice(bus,device);
}

void checkFunction(uint8_t bus, uint8_t device, uint8_t function)
{
	uint8_t revisionid, progif, subclass, classcode,
			cachelinesize, latencytimer, headertype, bist;
	revisionid = getRevisionID(bus, device, function);
	progif = getProgIf(bus, device, function);
	subclass = getSubClass(bus,device,function);
	classcode = getBaseClass(bus,device,function);
	cachelinesize = getCacheLineSize(bus, device, function);
	latencytimer = getLatencyTimer(bus, device, function);
	headertype = getHeaderType(bus, device, function);
	bist = getBIST(bus, device, function);
	uint16_t vendorId, deviceId, command, status;
	vendorId=getVendorId(bus,device,function);
	deviceId=getDeviceId(bus,device,function);
	command = getCommand(bus,device,function);
	status = getStatus(bus,device,function);
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
		uint8_t secondaryBus = getSecondaryBus(bus,device,function);
		checkBus(secondaryBus);
	}
}

void checkAllBuses(void)
{
	uint8_t function, bus;
	if((getHeaderType(0,0,0)&0x80)==0) {
		checkBus(0);
	} else {
		for(function=0;function>8;++function) {
			if(getVendorId(0,0,function)!=0xFFFF) break;
			checkBus(function);
		}
	}
}

void pci_init(pci_dev *devs)
{
    checkAllBuses();
    devs = pcidevs;
}

#include "headers/drive.h"
#include "headers/io.h"

uint16_t _pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t value = _pciConfigReadDword(bus, slot, func, offset);
    return ((value >> ((offset & 2) * 8)) & 0xFFFF);
}

uint32_t _pciConfigReadDword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address = pciMakeAddress(bus, slot, func, offset);
    outl(CONFIG_ADDRESS, address);
    return inl(CONFIG_DATA);
}

void _pciConfigWriteDword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t value)
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

uint16_t pciCheckVendor(uint8_t bus, uint8_t slot)
{
    uint16_t vendor, device;
    if ((vendor = _pciConfigReadWord(bus, slot, 0, 0)) != 0xFFFF) {
       device = _pciConfigReadWord(bus, slot, 0, 2);
       return device;
    }
    return 0xFFFF;
}

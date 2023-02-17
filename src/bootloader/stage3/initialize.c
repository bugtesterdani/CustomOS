#include "headers/initialize.h"

static uint32_t offset_addr = 0;

void InitializeAddresses(uint32_t address)
{
    if (offset_addr != 0)
    {
        return;
    }
    
    offset_addr = address;
    screen_setup_static(address + SCREEN_X, address + SCREEN_Y);
    ISR_setup_static(address + ISR_HANDLER);
    IDT_setup_static(address + IDT_IDT, address + IDT_DESC);
    GDT_setup_static(address + GDT_GDT, address + GDT_DESC);
    FAT32_setup_static(address + FAT32_INFOS);
    paging_setup_static(address + PAGING_DIR_ADDR);
}

#include "headers/initialize.h"

void InitializeAddresses(uint32_t address)
{
    screen_setup_static(address + SCREEN_X, address + SCREEN_Y);
    ISR_setup_static(address + ISR_HANDLER);
    IDT_setup_static(address + IDT_IDT, address + IDT_DESC);
    GDT_setup_static(address + GDT_GDT, address + GDT_DESC);
    ATA_setup_static(address + ATA_DEV_T, address + ATA_COUNT);
    FAT32_setup_static(address + FAT32_INFOS);
    paging_setup_static(address + PAGING_DIR_ADDR);
}

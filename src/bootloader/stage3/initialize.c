#include "headers/initialize.h"

void InitializeAddresses(uint32_t address)
{
    screen_setup_static(address + SCREEN_X, address + SCREEN_Y);
    ISR_setup_static(address + ISR_HANDLER);
    GDT_setup_static(address + GDT_GDT, address + GDT_DESC);
    ATA_setup_static(address + ATA_DEV_T, address + ATA_COUNT);
}

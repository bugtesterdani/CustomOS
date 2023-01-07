#include "headers/initialize.h"

#include "headers/screen.h"

void InitializeAddresses(uint32_t address)
{
    screen_setup_static(address + SCREEN_X, address + SCREEN_Y);
}

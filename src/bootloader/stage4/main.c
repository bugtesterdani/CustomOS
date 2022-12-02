#include "headers/stdint.h"
#include "headers/stdio.h"
#include "headers/fat32.h"

void _cdecl cstart_(uint16_t bootDrive)
{
    printstr("Hello from the C Code\r\n");
    for(;;);
}

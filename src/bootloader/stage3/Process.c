#include "headers/Process.h"
#include "headers/memory_management.h"
#include "headers/paging.h"

// #include "headers/colors.h"
// #include "headers/screen.h"
// #include "headers/string.h"
// #include "headers/stdio.h"
// #include "headers/commands.h"
// #include "headers/fat32.h"
// #include "headers/memory_management.h"
// #include "headers/keyboard.h"

void ProcessExecutor(uint32_t *address)
{
    void (*myfunc)();
    myfunc = (void*)(address);
    myfunc();
}

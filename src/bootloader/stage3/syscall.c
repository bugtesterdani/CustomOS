#include "headers/syscall.h"

#include "headers/stdio.h"
#include "headers/colors.h"
#include "headers/screen.h"

void syscall(registers_t *regs)
{
    disablePaging();
    setcursornewline();
    printString("Syscall Function called", Red, White);
    setcursornewline();
    enablePaging();
}

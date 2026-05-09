#include "syscalls.h"


void main()
{
    const char *msg = "Hello from user space via int 0x80\n";
    write(1, msg, 34);
}

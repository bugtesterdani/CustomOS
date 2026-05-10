#include "syscalls.h"

int write(int fd, const char* buffer, unsigned int count)
{
    int result;
    __asm__ __volatile__("int $0x80" : "=a"(result) : "a"(SYS_WRITE), "b"(fd), "c"(buffer), "d"(count));
    return result;
}

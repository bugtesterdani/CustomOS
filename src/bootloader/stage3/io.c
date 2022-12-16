#include "headers/io.h"

void outportb(unsigned int port,unsigned char value)
{
    __asm__ volatile ("outb %%al,%%dx"::"d" (port), "a" (value));
}

unsigned int inportb(unsigned int port)
{
    unsigned ret_val;
    __asm__ volatile ("inb %w1,%b0"    : "=a"(ret_val)    : "d"(port));
    return ret_val;
}

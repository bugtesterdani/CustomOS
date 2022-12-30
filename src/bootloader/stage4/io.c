#include "headers/io.h"
#include "headers/asm.h"

void outb(uint16_t port, uint8_t value)
{
    __asm__ __volatile__ ("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port)
{
    uint8_t value;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

void outw(uint16_t port, uint16_t value)
{
    __asm__ __volatile__ ("outw %0, %1" : : "a"(value), "Nd"(port));
}

uint16_t inw(uint16_t port)
{
    uint16_t value;
    __asm__ __volatile__ ("inw %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

void outl(uint16_t port, uint32_t value)
{
    __asm__ __volatile__ ("outl %0, %1" : : "a"(value), "Nd"(port));
}

uint32_t inl(uint16_t port)
{
    uint32_t value;
    __asm__ __volatile__ ("inl %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

// // Funktion zum Schreiben von 64-Bit-Werten in ein Hardware-Register
// void outq(unsigned short port, unsigned long long value)
// {
//     __asm__ __volatile__ ("outl %0, %1" : : "a"((unsigned int)(value & 0xFFFFFFFF)), "Nd"(port));
//     __asm__ __volatile__ ("outl %0, %1" : : "a"((unsigned int)(value >> 32)), "Nd"(port+4));
// }

// // Funktion zum Lesen von 64-Bit-Werten aus einem Hardware-Register
// unsigned long long inq(unsigned short port)
// {
//     unsigned long long value;
//     value = inl(port+4);
//     value = (value << 32) | inl(port);
//     return value;
// }

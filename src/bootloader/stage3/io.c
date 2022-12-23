#include "headers/io.h"
#include "headers/asm.h"

uint8_t asm_functions(io_inportb(uint16_t port));
void asm_functions(io_outportb(uint16_t port, uint8_t value));
uint16_t asm_functions(io_inportw(uint16_t port));
void asm_functions(io_outportw(uint16_t port, uint16_t value));
uint32_t asm_functions(io_inportd(uint16_t port));
void asm_functions(io_outportd(uint16_t port, uint32_t value));

void outportb(uint16_t port, uint8_t value)
{
    io_outportb(port, value);
}

void outportw(uint16_t port, uint16_t value)
{
    io_outportw(port, value);
}

void outportd(uint16_t port, uint32_t value)
{
    io_outportd(port, value);
}

uint8_t inportb(uint16_t port)
{
    return io_inportb(port);
}

uint16_t inportw(uint16_t port)
{
    return io_inportw(port);
}

uint32_t inportd(uint16_t port)
{
    return io_inportd(port);
}

void outl(unsigned short port, unsigned int value)
{
    __asm__ __volatile__ ("outl %0, %1" : : "a"(value), "Nd"(port));
}

unsigned int inl(unsigned short port)
{
    unsigned int value;
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

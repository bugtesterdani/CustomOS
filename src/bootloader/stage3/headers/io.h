#pragma once

#include "stdint.h"

uint8_t inportb(uint16_t port);
void outportb(uint16_t port, uint8_t value);
uint16_t inportw(uint16_t port);
void outportw(uint16_t port, uint16_t value);
uint32_t inportd(uint16_t port);
void outportd(uint16_t port, uint32_t value);

void outl(unsigned short port, unsigned int value);
unsigned int inl(unsigned short port);

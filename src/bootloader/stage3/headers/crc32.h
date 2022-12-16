#pragma once

#include "../headers/stdint.h"

#define INITXOR 0xFFFFFFFF
#define FINALXOR 0xFFFFFFFF

int crc32_tabledriven(unsigned char *buffer, int length);

#pragma once

#include "stdint.h"
#include <stdbool.h>

bool strcmp(char *str1, char *str2);
void strapp(char *str1, char *str2, uint8_t offset, uint8_t size);

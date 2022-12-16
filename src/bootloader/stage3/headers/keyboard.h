#pragma once

#include "keyboard_chars.h"

unsigned int FetchScancode();
unsigned int FetchAndAnalyzeScancode();
void ReadLine(char* buff, unsigned int length);
unsigned int parseKey(char Key);

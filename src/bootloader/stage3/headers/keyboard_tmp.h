#pragma once

#include "keyboard.h"

unsigned int tmp_FetchScancode();
unsigned int tmp_FetchAndAnalyzeScancode();
void tmp_ReadLine(char* buff, unsigned int length);
unsigned int tmp_parseKey(char Key);
char tmp_ReadChar();

void tmp_register_keyboard_callback();

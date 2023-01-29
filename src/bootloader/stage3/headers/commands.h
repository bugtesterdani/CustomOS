#pragma once

#include "stdint.h"

void ParseCommand(char* commandline);
void SplitParameters(char* line, int* start_pos, char* PartSplit);
void clearArray(uint8_t* Array, unsigned int length, char default_value);
void setstring(char* str, char* out);

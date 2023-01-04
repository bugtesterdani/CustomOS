#pragma once

#include "stdint.h"

void ParseCommand(char* commandline);
void SplitParameters(char* line, int* start_pos, char* PartSplit);
void clearArray(char* Array, unsigned int length, char default_value);
void setstring(char* str, char* out);

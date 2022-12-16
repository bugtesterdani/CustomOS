#pragma once

void ParseCommand(char* commandline);
void ConvertIntToChar(int value, char* Output);
int power(int base, unsigned int exp);
unsigned int Number(int value, int multiplicator);
char NumberToChar(int value);
void SplitParameters(char* line, int* start_pos, char* PartSplit);
void clearArray(char* Array, unsigned int length, char default_value);

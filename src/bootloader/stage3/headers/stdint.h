#pragma once

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed long int int32_t;
typedef unsigned long int uint32_t;
typedef signed long long int int64_t;
typedef unsigned long long int uint64_t;

void ConvertIntToChar(int value, char* Output);
void ConvertHexToChar(int value, char* Output);
int power(int base, unsigned int exp);
unsigned int Number(int value, int multiplicator);
unsigned int NumberHEX(int value, int multiplicator);
char NumberToChar(int value);
char NumberHEXToChar(int value);

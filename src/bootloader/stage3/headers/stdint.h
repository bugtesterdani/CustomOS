#pragma once

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed long int int32_t;
typedef unsigned long int uint32_t;
typedef signed long long int int64_t;
typedef unsigned long long int uint64_t;

typedef struct {
    uint8_t bit: 1;
} __attribute__((packed)) BIT1_t;
typedef struct {
    uint8_t bit: 2;
} __attribute__((packed)) BIT2_t;
typedef struct {
    uint8_t bit: 3;
} __attribute__((packed)) BIT3_t;
typedef struct {
    uint8_t bit: 4;
} __attribute__((packed)) BIT4_t;

void ConvertToChar(uint16_t value, int base, char* Output, int startindex);
uint64_t ParseStrInt(char* value, uint8_t base);
uint32_t pow(uint8_t base, uint8_t exponent);
uint8_t CharToNumber(char value);
char NumberToChar(int value);

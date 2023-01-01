#pragma once

#include "stdint.h"

typedef union
{
    uint64_t u64;
    int64_t s64;
    struct
    {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        uint32_t hi; uint32_t lo;
#else
        uint32_t lo; uint32_t hi;
#endif
    } u32;
    struct
    {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        int32_t hi; int32_t lo;
#else
        int32_t lo; int32_t hi;
#endif
    } s32;
} arith64_word;

#define arith64_hi(n) (arith64_word){.u64=n}.u32.hi
#define arith64_lo(n) (arith64_word){.u64=n}.u32.lo

uint64_t __udivdi3(uint64_t a, uint64_t b);
uint64_t __divmoddi4(uint64_t a, uint64_t b, uint64_t *c);
int32_t __clzdi2(uint64_t a);


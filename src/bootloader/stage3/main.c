#include "headers/stdio.h"
#include "headers/stdint.h"

#define Black 0x0
#define Blue 0x1
#define Green 0x2
#define Cyan 0x3
#define Red 0x4
#define Purple 0x5
#define Brown 0x6
#define Gray 0x7
#define Dark_Gray 0x8
#define Light_Blue 0x9
#define Light_Green 0xA
#define Light_Cyan 0xB
#define Light_Red 0xC
#define Light_Purple 0xD
#define Yellow 0xE
#define White 0xF

void _cstart_()
{
    // SP: $1 = (void *) 0x8ffe0
    //  RAX: 0x0000000000000000  RBX: 0x000000000000D60E  RCX: 0x0000000000000002  RDX: 0x00000000000B80BF  o d i t s z a P c 
    //  RSI: 0x000000000000849F  RDI: 0x0000000000008000  RBP: 0x000000000008FFF8  RSP: 0x000000000008FFE0  RIP: 0x0000000000008559
    //  R8 : 0x0000000000000000  R9 : 0x0000000000000000  R10: 0x0000000000000000  R11: 0x0000000000000000  R12: 0x0000000000000000
    //  R13: 0x0000000000000000  R14: 0x0000000000000000  R15: 0x0000000000000000
    //  CS: 0008  DS: 0010  ES: 0010  FS: 0010  GS: 0010  SS: 0010
    // 0x8ffe0:        0x00008621      0x00000003      0x00000007      0x00000004
    // 0x8fff0:        0x00000000      0x00000000      0x00090000      0x0000845e
    print("Hello from C Code", 3, Gray, Red);

    // endless loop
    for (;;);
}
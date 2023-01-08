#pragma once
#include "stdint.h"

#define IDT_Entries             256

typedef enum
{
    IDT_FLAG_GATE_TASK              = 0x5,
    IDT_FLAG_GATE_16BIT_INT         = 0x6,
    IDT_FLAG_GATE_16BIT_TRAP        = 0x7,
    IDT_FLAG_GATE_32BIT_INT         = 0xE,
    IDT_FLAG_GATE_32BIT_TRAP        = 0xF,

    IDT_FLAG_RING0                  = (0 << 5),
    IDT_FLAG_RING1                  = (1 << 5),
    IDT_FLAG_RING2                  = (2 << 5),
    IDT_FLAG_RING3                  = (3 << 5),

    IDT_FLAG_PRESENT                = 0x80
} IDT_FLAGS;

struct idt_entry_struct
{
    uint16_t BaseLow;                       // Segment Base (/Offset) Address (Bits 15-0)
    uint16_t SegmentSelector;               // Selector of the code where to jump when Interrupt triggered.
    uint8_t Reserved;                       // Will be set to 0
    // Flags:
    // 1 Bit (7):       Is Interrupt valid?
    // 2 Bit (65):      Descriptor privilege level
    // 1 Bit (4):       Always 0
    // 1 Bit (3):       Sets if it is an 16-bit or 32-bit Code inside
    // 3 Bit (210):     Sets the Type of this entry
    //                  Interrupt Gate: 110
    uint8_t Flags;
    uint16_t BaseHigh;                      // Segment Base (/Offset) Address (Bits 31-16)
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

struct idt_ptr_struct
{
    uint16_t Limit;
    idt_entry_t* Ptr;
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

#define FLAG_SET(x, flag) x |= (flag)
#define FLAG_UNSET(x, flag) x &= ~(flag)

void init_idt();
void idt_disable_gate(int interrupt);
void idt_enable_gate(int interrupt);
void idt_set_gate(int interrupt, void* base, uint16_t segmentDescriptor, uint8_t flags);
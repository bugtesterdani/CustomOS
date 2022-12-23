#include "headers/idt.h"
#include "headers/stdint.h"
#include "headers/asm.h"

idt_entry_t g_IDT[256];

idt_ptr_t g_IDTDescriptor = { sizeof(g_IDT) - 1, g_IDT };

void asm_functions(idt_flush(idt_ptr_t* idtDescriptor));

void idt_set_gate(int interrupt, void* base, uint16_t segmentDescriptor, uint8_t flags)
{
    g_IDT[interrupt].BaseLow = ((uint32_t)base) & 0xFFFF;
    g_IDT[interrupt].SegmentSelector = segmentDescriptor;
    g_IDT[interrupt].Reserved = 0;
    g_IDT[interrupt].Flags = flags;
    g_IDT[interrupt].BaseHigh = ((uint32_t)base >> 16) & 0xFFFF;
}

void idt_enable_gate(int interrupt)
{
    FLAG_SET(g_IDT[interrupt].Flags, IDT_FLAG_PRESENT);
}

void idt_disable_gate(int interrupt)
{
    FLAG_UNSET(g_IDT[interrupt].Flags, IDT_FLAG_PRESENT);
}

void init_idt()
{
    idt_flush(&g_IDTDescriptor);
}
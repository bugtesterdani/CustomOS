#pragma once

#include "stdint.h"

#define GDT_Entries             3

struct gdt_entry_struct
{
   // Segment Size (Bits 15-6)
   uint16_t limit_low;
   uint16_t base_low;         // Segment Base (/Offset) Address (Bits 15-0)
   uint8_t  base_middle;      // Segment Base (/Offset) Address (Bits 23-16)
   // Access:
   // 1 Bit (7):     Is Segment present in RAM
   // 2 Bit (65):    Descriptor privilege level
   // 1 Bit (4):     Descriptor Type (0 => System-; 1 => Normal-) Segment
   // 3 Bit (321):   Segment Type (Code or Data)
   //                000 	Datensegment (Schreibgeschützt)
   //                001 	Datensegment (Beschreibbar, Lesbar)
   //                010 	Reserviert (Nicht benutzen)
   //                011 	Datensegment (Expand-Down)
   //                100 	Codesegment (Nur ausführbar, nicht lesbar)
   //                101 	Codesegment (Ausführbar und lesbar)
   //                110 	Conforming Codesegment (Nur ausführbar, nicht lesbar)
   //                111 	Conforming Codesegment (Ausführbar und lesbar)
   // 1 Bit (0):     Is set by Processor automatically.
   //                If is an virtual Addressspace, this Bit say if it could be saved on hard disk.
   uint8_t  access;
   // Granularity:
   // 1 Bit (7):     Granularity (0 => 1Byte; 1 => 1kByte)
   // 1 Bit (6):     Operand Size (0 => 16-bit; 1 => 32-bit)
   // 1 Bit (5):     Always 0
   // 1 Bit (4):     Can be used by us. Will be ignored for now.
   // 4 Bit (3210):  Segment Size (Bits 19-16)
   uint8_t  granularity;
   uint8_t  base_high;        // Segment Base (/Offset) Address (Bits 31-24)
} __attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t;

struct gdt_ptr_struct
{
    uint16_t Limit;                     // sizeof(gdt) - 1
    gdt_entry_t* Ptr;                   // address of GDT
} __attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;

void init_gdt();
void GDT_setup_static(uint32_t address_GDT, uint32_t address_Descriptor);

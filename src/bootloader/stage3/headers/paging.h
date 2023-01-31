#pragma once

#include "stdint.h"
#include "isr.h"

#define PD_Entry(virt_addr) (uint32_t)(((virt_addr) >> 22) & 1024)
#define PT_Entry(virt_addr) (uint32_t)(((virt_addr) >> 12) % 1024)

struct Page
{
    uint8_t Present             :  1;
    uint8_t Writeable           :  1;
    uint8_t UserMode            :  1;
    uint8_t WriteThrough        :  1;
    uint8_t NoCaching           :  1;
    uint8_t UsedSinceLastCheck  :  1;
    uint8_t Written             :  1;
    uint8_t CPU_Res             :  2;
    uint8_t FreeBits            :  3;
    uint32_t Frame_Pointer      : 20;
} __attribute__((packed));

typedef struct Page PD_t;
typedef struct Page PT_t;

void asm_functions(enablePaging());
void asm_functions(disablePaging());

uint8_t paging_setup_newDirectory(uint32_t *address, uint16_t table_count);
void paging_setup_static(uint32_t address_page_directory);
void paging_setup();
uint8_t paging_setup_newTable(uint16_t entry_id);
void map_page(uint32_t *phys_address, uint32_t *virt_address, uint32_t flags);
void page_fault(registers_t* regs);

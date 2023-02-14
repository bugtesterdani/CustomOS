#include "headers/paging.h"
#include "headers/memory_management.h"
#include "headers/asm.h"

#include "headers/colors.h"
#include "headers/screen.h"
#include "headers/string.h"
#include "headers/stdio.h"
#include "headers/commands.h"
#include "headers/fat32.h"
#include "headers/keyboard.h"

static uint32_t *addr_page_dir;

void asm_functions(loadPageDirectory(uint32_t *page_directory));

void paging_setup_static(uint32_t address_page_directory)
{
    addr_page_dir = (uint32_t*)address_page_directory;
    *addr_page_dir = 0;
}

uint8_t paging_setup_newDirectory(uint32_t *address)
{
    uint32_t address_directory, offset_directory;
    if (allocate_block(&address_directory, &offset_directory, 1024 * 32) != 1)
    {
        return 0;
    }

    uint32_t addr = (address_directory + offset_directory);
    *address = addr;

    return 1;
}

uint8_t paging_setup_newTable(uint16_t entry_id)
{
    PD_t *Directory = (PD_t*)(*addr_page_dir);
    uint32_t address, offset;
    if (allocate_block(&address, &offset, 1024 * 32) != 1)
    {
        return 0;
    }
    printString("Setup new Directory Entry: ", White, Black);
    char output[80];
    clearArray(output, 80, 0x00);
    ConvertToChar((entry_id) & 0xFFFF, 16, output, 0);
    printString(output, White, Black);
    setcursornewline();
    clearArray(output, 80, 0x00);
    ConvertToChar(((address + offset) >> 16) & 0xFFFF, 16, output, 0);
    output[lastIndex(output, 80)] = ' ';
    ConvertToChar(((address + offset) >>  0) & 0xFFFF, 16, output, lastIndex(output, 80));
    printString(output, White, Black);
    setcursornewline();
    Directory[entry_id].FreeBits = 1;  // This will inidicates this Page shows to an address
    Directory[entry_id].Present = 1;
    Directory[entry_id].Writeable = 1;
    Directory[entry_id].UserMode = 1;
    Directory[entry_id].Frame_Pointer = ((address + offset) >> 12);
    return 1;
}

void paging_setup_newTableEntry(uint16_t d_entry_id, uint16_t t_entry_id, uint32_t address, uint32_t flags)
{
    printString("Setup new Table Entry: ", White, Black);
    char output[80];
    clearArray(output, 80, 0x00);
    ConvertToChar((t_entry_id) & 0xFFFF, 16, output, 0);
    printString(output, White, Black);
    clearArray(output, 80, 0x00);
    output[0] = ' ';
    ConvertToChar((d_entry_id) & 0xFFFF, 16, output, 1);
    printString(output, White, Black);
    setcursornewline();
    clearArray(output, 80, 0x00);
    ConvertToChar(((address) >> 16) & 0xFFFF, 16, output, 0);
    output[lastIndex(output, 80)] = ' ';
    ConvertToChar(((address) >>  0) & 0xFFFF, 16, output, lastIndex(output, 80));
    printString(output, White, Black);
    setcursornewline();
    PD_t *Directory = (PD_t*)(*addr_page_dir);
    PT_t *Table = (PT_t*)((Directory[d_entry_id].Frame_Pointer) << 12);
    *((uint32_t*)(&(Table[t_entry_id]))) = flags;
    clearArray(output, 80, 0x00);
    printString("ADDR: ", White, Black);
    ConvertToChar((((uint32_t)((uint32_t*)(&(Table[t_entry_id])))) >> 16) & 0xFFFF, 16, output, 0);
    ConvertToChar((((uint32_t)((uint32_t*)(&(Table[t_entry_id])))) >>  0) & 0xFFFF, 16, output, lastIndex(output, 80));
    printString(output, White, Black);
    setcursornewline();
    clearArray(output, 80, 0x00);
    printString("NEWADDR: ", White, Black);
    ConvertToChar((address >> 16) & 0xFFFF, 16, output, 0);
    ConvertToChar((address >>  0) & 0xFFFF, 16, output, lastIndex(output, 80));
    printString(output, White, Black);
    setcursornewline();
    Table[t_entry_id].FreeBits = 1;
    Table[t_entry_id].Frame_Pointer = ((address) >> 12);
}

void paging_setup()
{
    if (*addr_page_dir != 0)
    {
        return;
    }

    uint32_t new_address;
    paging_setup_newDirectory(&new_address);
    *addr_page_dir = new_address;

    // Paging register IRQ
    irq_register(0x0E, &page_fault);

    loadPageDirectory((uint32_t*)(*addr_page_dir));
}

void map_page(uint32_t *phys_address, uint32_t *virt_address, uint32_t flags)
{
    uint16_t PD = PD_Entry(*virt_address);
    uint16_t PT = PT_Entry(*virt_address);

    PD_t *Directory = (PD_t*)(*addr_page_dir);
    if (Directory[PD].FreeBits == 0)
    {
        paging_setup_newTable(PD);
    }
    PT_t *Table = (PT_t*)((Directory[PD].Frame_Pointer) << 12);
    if (Table[PT].FreeBits == 0)
    {
        printString(" set new table ", White, Black);
        paging_setup_newTableEntry(PD, PT, *phys_address, flags);
    }
    else
    {
        printString(" no new table set ", White, Black);
        Table[PT].Frame_Pointer = ((*phys_address) >> 12);
    }
    setcursornewline();
}

void page_fault(registers_t* regs)
{
    disablePaging();

    // Now we could handle the fault
    printString("Page fault handled ...", White, Black);

    enablePaging();
}

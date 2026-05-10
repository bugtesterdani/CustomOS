#include "headers/paging.h"
#include "headers/memory_management.h"
#include "headers/asm.h"

#include "headers/colors.h"
#include "headers/screen.h"
#include "headers/string.h"
#include "headers/stdio.h"
#include "headers/fat32.h"

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
    Directory[entry_id].FreeBits = 1;  // This will inidicates this Page shows to an address
    Directory[entry_id].Present = 1;
    Directory[entry_id].Writeable = 1;
    Directory[entry_id].UserMode = 1;
    Directory[entry_id].Frame_Pointer = ((address + offset) >> 12);
    return 1;
}

void paging_setup_newTableEntry(uint16_t d_entry_id, uint16_t t_entry_id, uint32_t address, uint32_t flags)
{
    PD_t *Directory = (PD_t*)(*addr_page_dir);
    PT_t *Table = (PT_t*)((Directory[d_entry_id].Frame_Pointer) << 12);
    *((uint32_t*)(&(Table[t_entry_id]))) = flags;
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
    if (paging_setup_newDirectory(&new_address) == 0)
    {
        return;
    }
    *addr_page_dir = new_address;

    uint32_t flags = 0x003;
    uint32_t page_directory = *addr_page_dir;

    // Identity map first 4MB to keep currently executing code/data/stack valid.
    for (uint32_t i = 0; i < (1024 * 1024 * 4); i += 4096)
    {
        uint32_t phys = i;
        uint32_t virt = i;
        map_page((uint32_t*)phys, (uint32_t*)virt, flags);
    }

    // Ensure VGA memory is identity mapped.
    uint32_t vga = 0xB8000;
    map_page((uint32_t*)vga, (uint32_t*)vga, flags);

    // Ensure page directory and all active page tables are identity mapped.
    map_page((uint32_t*)page_directory, (uint32_t*)page_directory, flags);
    PD_t *Directory = (PD_t*)page_directory;
    for (uint16_t i = 0; i < 1024; i++)
    {
        if (Directory[i].Present == 1)
        {
            uint32_t table_addr = (Directory[i].Frame_Pointer << 12);
            map_page((uint32_t*)table_addr, (uint32_t*)table_addr, flags);
        }
    }

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
        paging_setup_newTableEntry(PD, PT, *phys_address, flags);
    }
    else
    {
        *((uint32_t*)(&(Table[PT]))) = flags;
        Table[PT].Present = 1;
        Table[PT].Writeable = 1;
        Table[PT].Frame_Pointer = ((*phys_address) >> 12);
    }
}

void page_fault(registers_t* regs)
{
    disablePaging();

    // Now we could handle the fault
    printString("Page fault handled ...", White, Black);

    enablePaging();
}

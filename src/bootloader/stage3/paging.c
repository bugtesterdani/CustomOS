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

uint8_t paging_setup_newDirectory(uint32_t *address, uint16_t table_count)
{
    uint32_t address_directory, offset_directory;
    if (allocate_block(&address_directory, &offset_directory, 1024 * 32) != 1)
    {
        return 0;
    }

    uint32_t addr = (address_directory + offset_directory);
    *address = addr;

    uint32_t *directory = (uint32_t*)addr;
    uint32_t value = (1 + 2 + 4);

    // // Try to allocate Table Count Amount
    // for (uint32_t i = 0; i < table_count; i++)
    // {
    //     uint32_t address_table, offset_table;
    //     if (allocate_block(&address_table, &offset_table, 1024 * 32) != 1)
    //     {
    //         return 2;
    //     }
    //     directory[i] = (uint32_t)(address_table + offset_table);

    //     PD_t *Directory = (PD_t*)(&(directory[i]));
    //     Directory->Present = 1;
    //     Directory->Writeable = 1;
    //     Directory->UserMode = 1;

    //     Directory = (PD_t*)(directory[i]);
    //     Directory->Frame_Pointer = (address_table + offset_table);

    //     uint32_t *table_entry = (uint32_t*)(address_table + offset_table);

    //     for (uint16_t j = 0; j < 1024; j++)
    //     {
    //         table_entry[j] = value;
    //         value += 4096;
    //     }
    // }

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
    Directory[entry_id].Present = 1;
    Directory[entry_id].Writeable = 1;
    Directory[entry_id].UserMode = 1;
    Directory[entry_id].Frame_Pointer = (address + offset);
    return 1;
}

void paging_setup()
{
    if (*addr_page_dir != 0)
    {
        return;
    }

    uint32_t new_address;
    paging_setup_newDirectory(&new_address, 1);
    *addr_page_dir = new_address;

    // Paging register IRQ
    irq_register(0x0E, &page_fault);

    loadPageDirectory((uint32_t*)new_address);
}

void map_page(uint32_t *phys_address, uint32_t *virt_address, uint32_t flags)
{
    uint32_t PD = PD_Entry(*virt_address);
    uint32_t PT = PT_Entry(*virt_address);

    uint32_t *Directory = (uint32_t*)(virt_address[PD]);
    uint32_t *Page = (uint32_t*)(virt_address[PT]);

    *Page = (uint32_t)phys_address + flags;
}

void page_fault(registers_t* regs)
{
    disablePaging();

    // Now we could handle the fault


    enablePaging();
}

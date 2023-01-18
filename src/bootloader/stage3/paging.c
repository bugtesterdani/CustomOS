#include "headers/paging.h"
#include "headers/memory_management.h"
#include "headers/asm.h"
#include "headers/isr.h"

static uint32_t *page_directory = (uint32_t*)0;
static uint32_t *addr_page_dir;

void asm_functions(loadPageDirectory(uint32_t *page_directory));
void asm_functions(enablePaging());

void paging_setup_static(uint32_t address_page_directory)
{
    addr_page_dir = (uint32_t*)address_page_directory;
}

void paging_setup()
{
    char Buff[60];
    if (page_directory != (uint32_t*)0)
    {
        return;
    }

    output_mem_things();
    setcursornewline();
    ReadLine(Buff, 60);

    uint16_t page_dir_entries = 1024;
    uint32_t address_dir, offset_dir;
    if (allocate_block(&address_dir, &offset_dir, page_dir_entries * 32) == 0) // Its only 32 bit per Entry
    {
        // Failed to allocate space
        for(;;);
        return;
    }

    output_mem_things();
    setcursornewline();
    ReadLine(Buff, 60);

    page_directory = (uint32_t*)(address_dir + offset_dir);
    *addr_page_dir = (address_dir + offset_dir);

    for (uint16_t i = 0; i < page_dir_entries; i++)
    {
        page_directory[i] = 0x00000002;
    }

    // setup first page table

    uint16_t page_table_entries = 1024;
    uint32_t address_table, offset_table;
    if (allocate_block(&address_table, &offset_table, page_table_entries * 32) == 0) // Its only 32 bit per Entry
    {
        // Failed to allocate space
        for(;;);
        return;
    }

    output_mem_things();
    setcursornewline();
    ReadLine(Buff, 60);

    uint32_t *page_table = (uint32_t*)(address_table + offset_table);
    for (uint16_t i = 0; i < page_table_entries; i++)
    {
        page_table[i] = (i * 0x1000) | 3;
    }

    // put first page table into page directory
    page_directory[0] = ((uint32_t)page_table) | 3;

    // load page directory to cpu and enable paging
    loadPageDirectory(page_directory);
    enablePaging();

    output_mem_things();
    setcursornewline();
    ReadLine(Buff, 60);
}

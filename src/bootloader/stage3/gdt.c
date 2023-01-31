#include "headers/gdt.h"
#include "headers/stdint.h"
#include "headers/asm.h"

typedef enum
{
    GDT_ACCESS_CODE_READABLE                = 0x02,
    GDT_ACCESS_DATA_WRITEABLE               = 0x02,

    GDT_ACCESS_CODE_CONFORMING              = 0x04,
    GDT_ACCESS_DATA_DIRECTION_NORMAL        = 0x00,
    GDT_ACCESS_DATA_DIRECTION_DOWN          = 0x04,

    GDT_ACCESS_DATA_SEGMENT                 = 0x10,
    GDT_ACCESS_CODE_SEGMENT                 = 0x18,

    GDT_ACCESS_DESCRIPTOR_TSS               = 0x00,

    GDT_ACCESS_RING0                        = 0x00,
    GDT_ACCESS_RING1                        = 0x20,
    GDT_ACCESS_RING2                        = 0x40,
    GDT_ACCESS_RING3                        = 0x60,

    GDT_ACCESS_PRESENT                      = 0x80,

} GDT_ACCESS;

typedef enum 
{
    GDT_FLAG_64BIT                          = 0x20,
    GDT_FLAG_32BIT                          = 0x40,
    GDT_FLAG_16BIT                          = 0x00,

    GDT_FLAG_GRANULARITY_1B                 = 0x00,
    GDT_FLAG_GRANULARITY_4K                 = 0x80,
} GDT_FLAGS;

// Helper macros
#define GDT_LIMIT_LOW(limit)                (limit & 0xFFFF)
#define GDT_BASE_LOW(base)                  (base & 0xFFFF)
#define GDT_BASE_MIDDLE(base)               ((base >> 16) & 0xFF)
#define GDT_FLAGS_LIMIT_HI(limit, flags)    (((limit >> 16) & 0xF) | (flags & 0xF0))
#define GDT_BASE_HIGH(base)                 ((base >> 24) & 0xFF)

#define GDT_ENTRY(base, limit, access, flags) {                     \
    GDT_LIMIT_LOW(limit),                                           \
    GDT_BASE_LOW(base),                                             \
    GDT_BASE_MIDDLE(base),                                          \
    access,                                                         \
    GDT_FLAGS_LIMIT_HI(limit, flags),                               \
    GDT_BASE_HIGH(base)                                             \
}

static gdt_entry_t *g_GDT;
static gdt_ptr_t *g_GDTDescriptor;

// gdt_entry_t g_GDT[] = {
//     // NULL descriptor
//     GDT_ENTRY(0, 0, 0, 0),

//     // Kernel 32-bit code segment
//     GDT_ENTRY(0,
//               0xFFFFF,
//               GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_CODE_SEGMENT | GDT_ACCESS_CODE_READABLE,
//               GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K),

//     // Kernel 32-bit data segment
//     GDT_ENTRY(0,
//               0xFFFFF,
//               GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_DATA_SEGMENT | GDT_ACCESS_DATA_WRITEABLE,
//               GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K),

// };

// gdt_ptr_t g_GDTDescriptor = { sizeof(g_GDT) - 1, g_GDT};

void asm_functions(gdt_flush(gdt_ptr_t* descriptor, uint16_t codeSegment, uint16_t dataSegment));

void GDT_setup_static(uint32_t address_GDT, uint32_t address_Descriptor)
{
    g_GDT = (gdt_entry_t*)address_GDT;
    g_GDTDescriptor = (gdt_ptr_t*)address_Descriptor;
}

void init_gdt()
{
    g_GDT[0] = (gdt_entry_t)GDT_ENTRY(0, 0, 0, 0);
    g_GDT[1] = (gdt_entry_t)GDT_ENTRY(0, 0xFFFFFFFF,
                    GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_CODE_SEGMENT | GDT_ACCESS_CODE_READABLE,
                    GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K);
    g_GDT[2] = (gdt_entry_t)GDT_ENTRY(0, 0xFFFFFFFF,
                    GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_DATA_SEGMENT | GDT_ACCESS_DATA_WRITEABLE,
                    GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K);
    
    *g_GDTDescriptor = (gdt_ptr_t) { (GDT_Entries * sizeof(gdt_entry_t)) - 1, g_GDT};
    
    gdt_flush(g_GDTDescriptor, 0x08, 0x10);
}
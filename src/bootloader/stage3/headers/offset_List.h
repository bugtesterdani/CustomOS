#pragma once

#include "screen.h"
#include "isr.h"
#include "idt.h"
#include "gdt.h"
#include "ata.h"
#include "fat32.h"
#include "paging.h"

#define SCREEN_X        0                                                   // uint8_t
#define SCREEN_Y        SCREEN_X + (sizeof(uint8_t))                        // uint8_t
#define ISR_HANDLER     SCREEN_Y + (sizeof(uint8_t))                        // 256 * ISR_HANDLER
#define IDT_IDT         ISR_HANDLER + ((sizeof(ISR_HANDLER) * 256))         // 256 * idt_entry_t
#define IDT_DESC        IDT_IDT + ((sizeof(idt_entry_t) * IDT_Entries))     // ptr
#define GDT_GDT         IDT_DESC + sizeof(idt_ptr_t)                        // 256 * gdt_entry_t
#define GDT_DESC        GDT_GDT + ((sizeof(gdt_entry_t) * GDT_Entries))     // ptr
#define ATA_DEV_T       GDT_DESC + sizeof(gdt_ptr_t)                        // 4 * ATA_t
#define ATA_COUNT       ATA_DEV_T + (4 * sizeof(ATA_t))                     // uint8_t
#define FAT32_INFOS     ATA_COUNT + (sizeof(uint8_t))                       // uint32_t to show at page with readed informations
#define PAGING_DIR_ADDR FAT32_INFOS + (sizeof(uint32_t))                    // uint32_t to show at page where the Page directory is located

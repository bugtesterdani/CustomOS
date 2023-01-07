#pragma once

#include "stdint.h"
#include "arith.h"      // We will divide through 1000 a uint64_t Variable

#define RAM_OFFSET 0x500
    // 0: Unbekannter Typ
    // 1: Verfügbarer Speicher
    // 2: Reservierter Speicher
    // 3: Nicht verfügbarer Speicher (ACPI-Rechnerressourcen)
    // 4: Speicher, der als "Fehlerhafte Sektoren" markiert wurde
#define RAM_Type_Unknown        0x00
#define RAM_Type_Available      0x01
#define RAM_Type_Reserved       0x02
#define RAM_Type_ACPI           0x03
#define RAM_Type_Error          0x04

struct RAM
{
    uint64_t BaseAddress;
    uint64_t Size;
    uint32_t Type;
    uint32_t ExtendedPart;
} __attribute__((packed));

typedef struct RAM RAM_t;

void RAM_FullInit();

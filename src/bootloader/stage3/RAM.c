#include "headers/RAM.h"
#include "headers/memory_management.h"

void RAM_FullInit()
{
    uint8_t* values = (uint8_t*)RAM_OFFSET;
    uint8_t size_RAM = values[0];
    RAM_t* RAM = (RAM_t*)(RAM_OFFSET + 0x04 + (size_RAM * 0x18));

    uint8_t output[80];
    uint8_t _lastindex;
    for (int i = 0; i < values[0]; i++)
    {
        // Lese Eintrag aus Speicher
        RAM[i].BaseAddress = *(uint64_t*)(RAM_OFFSET + 0x04 + i * 0x18);
        RAM[i].Size = *(uint64_t*)(RAM_OFFSET + 0x0C + i * 0x18);
        RAM[i].Type = *(uint32_t*)(RAM_OFFSET + 0x14 + i * 0x18);
        RAM[i].ExtendedPart = *(uint32_t*)(RAM_OFFSET + 0x18 + i * 0x18);
        // Verarbeite Eintrag ...
    }

    uint32_t* _memory_pointer = (uint32_t*)RAM_OFFSET;
    for (uint32_t i = 0; i < ((values[0] * 0x06) + 0x01); i++)
    {
        _memory_pointer[i] = (uint32_t)0x00;
    }

    uint64_t sizeBytes_usedKernel = 0;
    uint64_t sizeBytes_Available = 0;
    uint64_t sizeBytes_Total = 0;

    for (int i = 0; i < size_RAM; i++)
    {
        // RAM_t RAM = RAM_Values[i];
        RAM_t RAM_Value = RAM[i];
        for (uint8_t j = 0; j < 0x06; j++)
        {
            _memory_pointer[((values[0] * 0x06) + 0x01) + j] = (uint32_t)0x00;
        }
        uint8_t tmp_char[80];
        switch (RAM_Value.Type)
        {
            case RAM_Type_Available:
                if (RAM_Value.BaseAddress == 0x00)
                {
                    sizeBytes_usedKernel = RAM_Value.Size;
                }
                else
                {
                    register_memspace(&RAM_Value);
                    sizeBytes_Available += RAM_Value.Size;
                }
                sizeBytes_Total += RAM_Value.Size;
                break;
            
            case RAM_Type_Reserved:
                sizeBytes_Total += RAM_Value.Size;
                break;

            case RAM_Type_ACPI:
                sizeBytes_Total += RAM_Value.Size;
                break;

            case RAM_Type_Error:
                sizeBytes_Total += RAM_Value.Size;
                break;

            default:
                sizeBytes_Total += RAM_Value.Size;
                break;
        }
    }

    register_memory_map();
}

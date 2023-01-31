#pragma once

#include "stdint.h"

// ELF Header Speicifcation
// Position (32 bit)    Position (64 bit)   Value
// 0-3                  0-3                 Magic number - 0x7F, then 'ELF' in ASCII
// 4                    4                   1 = 32 bit, 2 = 64 bit
// 5                    5                   1 = little endian, 2 = big endian
// 6                    6                   ELF header version
// 7                    7                   OS ABI - usually 0 for System V
// 8-15 	            8-15                Unused/padding
// 16-17                16-17               1 = relocatable, 2 = executable, 3 = shared, 4 = core
// 18-19                18-19               Instruction set - see table below
// 20-23                20-23               ELF Version
// 24-27                24-31               Program entry position
// 28-31                32-39               Program header table position
// 32-35                40-47               Section header table position
// 36-39                48-51               Flags - architecture dependent; see note below
// 40-41                52-53               Header size
// 42-43                54-55               Size of an entry in the program header table
// 44-45                56-57               Number of entries in the program header table
// 46-47                58-59               Size of an entry in the section header table
// 48-49                60-61               Number of entries in the section header table
// 50-51                62-63               Index in section header table with the section names
typedef struct ELF_header
{
    uint8_t MagicNumber[4];
    uint8_t BitVersion;
    uint8_t Endianess;
    uint8_t HeaderVersion;
    uint8_t OS_ABI;
    uint8_t unused[8];
    uint16_t FileType;
    uint16_t InstructionSet;
    uint8_t ELFVersion[4];
    uint8_t ProgramEntryPosition[4];
    uint8_t HeaderTablePosition[4];
    uint8_t SectionHeaderTablePosition[4];
    uint8_t FLAGS[4];
    uint16_t HeaderSize;
    uint16_t SizePerProgramHeaderTableEntry;
    uint16_t NumberOfProgramHeaderTableEntries;
    uint16_t SizePerSectionHeaderTableEntry;
    uint16_t NumberOfSectionHeaderTableEntries;
    uint16_t IndexSectionHeaderSectionNames;
} __attribute__((packed)) ELF_header_t;

uint8_t parseELFFile(uint16_t *elffile, uint32_t sizeBytes, uint16_t virtual_address);
void Parsing(char *name, uint32_t offset_block);

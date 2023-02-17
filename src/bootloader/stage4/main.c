#include "headers/idt.h"
#include "headers/screen.h"

#define PAGING_DISABLE 0x20

#define Black 0x0
#define Blue 0x1
#define Green 0x2
#define Cyan 0x3
#define Red 0x4
#define Purple 0x5
#define Brown 0x6
#define Gray 0x7
#define Dark_Gray 0x8
#define Light_Blue 0x9
#define Light_Green 0xA
#define Light_Cyan 0xB
#define Light_Red 0xC
#define Light_Purple 0xD
#define Yellow 0xE
#define White 0xF

#define IDT_IDT         0xc0000000
#define IDT_DESC        IDT_IDT + ((sizeof(idt_entry_t) * IDT_Entries))     // ptr
#define SCREEN_Y        IDT_DESC + sizeof(idt_ptr_t)                        // 256 * gdt_entry_t
#define SCREEN_X        SCREEN_Y + sizeof(uint8_t)

// typedef void func(char *, unsigned char, unsigned char);
typedef void func(void);

void main()
{
    IDT_setup_static(IDT_IDT, IDT_DESC);
    screen_setup_static(SCREEN_X, SCREEN_Y);
    isr_init();
    irq_init();
    init_idt();
    clearscreen();
    char output[80] = "Hello World from the Kernel";
    uint8_t Background = Black;
    uint8_t Foreground = White;
    for (uint8_t i = 0; i < 80; i++)
    {
        if (output[i] == 0x00)
        {
            i = 80;
        }
        else
        {
            putchar(output[i], (Background << 4) | Foreground);
        }
    }
    for(;;);
}

#include "headers/idt.h"
#include "headers/gdt.h"
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
#define GDT_GDT         SCREEN_X + sizeof(uint8_t)
#define GDT_DESC        GDT_GDT + sizeof((sizeof(gdt_entry_t) * 3))
#define ISR_HANDLER     GDT_DESC + sizeof(gdt_ptr_t)

// typedef void func(char *, unsigned char, unsigned char);
typedef void func(void);

void testing_idt();
void running_interrupt30(registers_t *regs);
void running_interrupt40(registers_t *regs);

uint16_t get_cpl() {
    uint16_t cpl;

    __asm__ __volatile__ ("mov %%cs, %0" : "=r" (cpl));

    return cpl & 0x3; // Maskiere die unteren 2 Bits
}

void main()
{
    for (uint16_t i = 0; i < 0xf000; i += sizeof(uint32_t))
    {
        uint32_t *value = (uint32_t*)(0xc0000000 + i);
        *value = 0x00000000;
    }
    IDT_setup_static(IDT_IDT, IDT_DESC);
    screen_setup_static(SCREEN_X, SCREEN_Y);
    GDT_setup_static(GDT_GDT, GDT_DESC);
    ISR_setup_static(ISR_HANDLER);
    init_gdt();
    isr_init();
    isr_register(0x81, running_interrupt40);
    isr_register(0x82, running_interrupt30);
    irq_init();
    init_idt();
    __asm__ __volatile__ ("cli");
    enable_interrupts();
    clearscreen();
    printString("Hello World from the kernel part", White, Black);
    //testing_idt();
    char charout[80];
    ConvertToChar(get_cpl(), 16, charout, 0);
    printString(charout, 0xf, 0x0);
    // uint32_t *tmp = (uint32_t*)0x100000;
    // *tmp = 100;
    __asm__ __volatile__("int $0x82");
    __asm__ __volatile__("int $0x81");
    __asm__ __volatile__("int $0x80");
    for(;;);
}

void running_interrupt30(registers_t *regs)
{
    setcursornewline();
    printString("Interrupt received 30", White, Black);
    setcursornewline();
}

void running_interrupt40(registers_t *regs)
{
    setcursornewline();
    printString("Interrupt received - 40", White, Black);
    setcursornewline();
}

struct idt
{
    void *base;
    unsigned short length;
};

void testing_idt()
{
    struct idt idt;
    __asm__ __volatile__ ("sidt %0" : "=m"(idt));
}

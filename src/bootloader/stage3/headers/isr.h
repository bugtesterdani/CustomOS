#pragma once
#include "stdint.h"
#include "asm.h"

#define IRQ_OFFSET 32
#define IQ0                     (IRQ_OFFSET + 0)
#define IQ_KEYBOARD             (IRQ_OFFSET + 1)
#define IQ2                     (IRQ_OFFSET + 2)
#define IQ3                     (IRQ_OFFSET + 3)
#define IQ4                     (IRQ_OFFSET + 4)
#define IQ5                     (IRQ_OFFSET + 5)
#define IQ6                     (IRQ_OFFSET + 6)
#define IQ7                     (IRQ_OFFSET + 7)
#define IQ8                     (IRQ_OFFSET + 8)
#define IQ9                     (IRQ_OFFSET + 9)
#define IQ10                    (IRQ_OFFSET + 10)
#define IQ11                    (IRQ_OFFSET + 11)
#define IQ12                    (IRQ_OFFSET + 12)
#define IQ13                    (IRQ_OFFSET + 13)
#define IQ14                    (IRQ_OFFSET + 14)
#define IQ15                    (IRQ_OFFSET + 15)

struct registers
{
    // in the reverse order they are pushed:
    uint32_t ds;                                            // data segment pushed by us
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;        // pusha
    uint32_t int_no, error_code;                            // we push interrupt, error is pushed automatically (or our dummy)
    uint32_t eip, cs, eflags, useresp, ss;                  // pushed automatically by CPU
} __attribute__((packed));

typedef struct registers registers_t;

typedef void (*ISRHandler)(registers_t* regs);

void asm_functions(ISR0());
void asm_functions(ISR1());
void asm_functions(ISR2());
void asm_functions(ISR3());
void asm_functions(ISR4());
void asm_functions(ISR5());
void asm_functions(ISR6());
void asm_functions(ISR7());
void asm_functions(ISR8());
void asm_functions(ISR9());
void asm_functions(ISR10());
void asm_functions(ISR11());
void asm_functions(ISR12());
void asm_functions(ISR13());
void asm_functions(ISR14());
void asm_functions(ISR15());
void asm_functions(ISR16());
void asm_functions(ISR17());
void asm_functions(ISR18());
void asm_functions(ISR19());
void asm_functions(ISR20());
void asm_functions(ISR21());
void asm_functions(ISR22());
void asm_functions(ISR23());
void asm_functions(ISR24());
void asm_functions(ISR25());
void asm_functions(ISR26());
void asm_functions(ISR27());
void asm_functions(ISR28());
void asm_functions(ISR29());
void asm_functions(ISR30());
void asm_functions(ISR31());
void asm_functions(ISR32());
void asm_functions(ISR33());
void asm_functions(ISR34());
void asm_functions(ISR35());
void asm_functions(ISR36());
void asm_functions(ISR37());
void asm_functions(ISR38());
void asm_functions(ISR39());
void asm_functions(ISR40());
void asm_functions(ISR41());
void asm_functions(ISR42());
void asm_functions(ISR43());
void asm_functions(ISR44());
void asm_functions(ISR45());
void asm_functions(ISR46());
void asm_functions(ISR47());
void asm_functions(ISR48());
void asm_functions(ISR49());
void asm_functions(ISR50());
void asm_functions(ISR51());
void asm_functions(ISR52());
void asm_functions(ISR53());
void asm_functions(ISR54());
void asm_functions(ISR55());
void asm_functions(ISR56());
void asm_functions(ISR57());
void asm_functions(ISR58());
void asm_functions(ISR59());
void asm_functions(ISR60());
void asm_functions(ISR61());
void asm_functions(ISR62());
void asm_functions(ISR63());
void asm_functions(ISR64());
void asm_functions(ISR65());
void asm_functions(ISR66());
void asm_functions(ISR67());
void asm_functions(ISR68());
void asm_functions(ISR69());
void asm_functions(ISR70());
void asm_functions(ISR71());
void asm_functions(ISR72());
void asm_functions(ISR73());
void asm_functions(ISR74());
void asm_functions(ISR75());
void asm_functions(ISR76());
void asm_functions(ISR77());
void asm_functions(ISR78());
void asm_functions(ISR79());
void asm_functions(ISR80());
void asm_functions(ISR81());
void asm_functions(ISR82());
void asm_functions(ISR83());
void asm_functions(ISR84());
void asm_functions(ISR85());
void asm_functions(ISR86());
void asm_functions(ISR87());
void asm_functions(ISR88());
void asm_functions(ISR89());
void asm_functions(ISR90());
void asm_functions(ISR91());
void asm_functions(ISR92());
void asm_functions(ISR93());
void asm_functions(ISR94());
void asm_functions(ISR95());
void asm_functions(ISR96());
void asm_functions(ISR97());
void asm_functions(ISR98());
void asm_functions(ISR99());
void asm_functions(ISR100());
void asm_functions(ISR101());
void asm_functions(ISR102());
void asm_functions(ISR103());
void asm_functions(ISR104());
void asm_functions(ISR105());
void asm_functions(ISR106());
void asm_functions(ISR107());
void asm_functions(ISR108());
void asm_functions(ISR109());
void asm_functions(ISR110());
void asm_functions(ISR111());
void asm_functions(ISR112());
void asm_functions(ISR113());
void asm_functions(ISR114());
void asm_functions(ISR115());
void asm_functions(ISR116());
void asm_functions(ISR117());
void asm_functions(ISR118());
void asm_functions(ISR119());
void asm_functions(ISR120());
void asm_functions(ISR121());
void asm_functions(ISR122());
void asm_functions(ISR123());
void asm_functions(ISR124());
void asm_functions(ISR125());
void asm_functions(ISR126());
void asm_functions(ISR127());
void asm_functions(ISR128());
void asm_functions(ISR129());
void asm_functions(ISR130());
void asm_functions(ISR131());
void asm_functions(ISR132());
void asm_functions(ISR133());
void asm_functions(ISR134());
void asm_functions(ISR135());
void asm_functions(ISR136());
void asm_functions(ISR137());
void asm_functions(ISR138());
void asm_functions(ISR139());
void asm_functions(ISR140());
void asm_functions(ISR141());
void asm_functions(ISR142());
void asm_functions(ISR143());
void asm_functions(ISR144());
void asm_functions(ISR145());
void asm_functions(ISR146());
void asm_functions(ISR147());
void asm_functions(ISR148());
void asm_functions(ISR149());
void asm_functions(ISR150());
void asm_functions(ISR151());
void asm_functions(ISR152());
void asm_functions(ISR153());
void asm_functions(ISR154());
void asm_functions(ISR155());
void asm_functions(ISR156());
void asm_functions(ISR157());
void asm_functions(ISR158());
void asm_functions(ISR159());
void asm_functions(ISR160());
void asm_functions(ISR161());
void asm_functions(ISR162());
void asm_functions(ISR163());
void asm_functions(ISR164());
void asm_functions(ISR165());
void asm_functions(ISR166());
void asm_functions(ISR167());
void asm_functions(ISR168());
void asm_functions(ISR169());
void asm_functions(ISR170());
void asm_functions(ISR171());
void asm_functions(ISR172());
void asm_functions(ISR173());
void asm_functions(ISR174());
void asm_functions(ISR175());
void asm_functions(ISR176());
void asm_functions(ISR177());
void asm_functions(ISR178());
void asm_functions(ISR179());
void asm_functions(ISR180());
void asm_functions(ISR181());
void asm_functions(ISR182());
void asm_functions(ISR183());
void asm_functions(ISR184());
void asm_functions(ISR185());
void asm_functions(ISR186());
void asm_functions(ISR187());
void asm_functions(ISR188());
void asm_functions(ISR189());
void asm_functions(ISR190());
void asm_functions(ISR191());
void asm_functions(ISR192());
void asm_functions(ISR193());
void asm_functions(ISR194());
void asm_functions(ISR195());
void asm_functions(ISR196());
void asm_functions(ISR197());
void asm_functions(ISR198());
void asm_functions(ISR199());
void asm_functions(ISR200());
void asm_functions(ISR201());
void asm_functions(ISR202());
void asm_functions(ISR203());
void asm_functions(ISR204());
void asm_functions(ISR205());
void asm_functions(ISR206());
void asm_functions(ISR207());
void asm_functions(ISR208());
void asm_functions(ISR209());
void asm_functions(ISR210());
void asm_functions(ISR211());
void asm_functions(ISR212());
void asm_functions(ISR213());
void asm_functions(ISR214());
void asm_functions(ISR215());
void asm_functions(ISR216());
void asm_functions(ISR217());
void asm_functions(ISR218());
void asm_functions(ISR219());
void asm_functions(ISR220());
void asm_functions(ISR221());
void asm_functions(ISR222());
void asm_functions(ISR223());
void asm_functions(ISR224());
void asm_functions(ISR225());
void asm_functions(ISR226());
void asm_functions(ISR227());
void asm_functions(ISR228());
void asm_functions(ISR229());
void asm_functions(ISR230());
void asm_functions(ISR231());
void asm_functions(ISR232());
void asm_functions(ISR233());
void asm_functions(ISR234());
void asm_functions(ISR235());
void asm_functions(ISR236());
void asm_functions(ISR237());
void asm_functions(ISR238());
void asm_functions(ISR239());
void asm_functions(ISR240());
void asm_functions(ISR241());
void asm_functions(ISR242());
void asm_functions(ISR243());
void asm_functions(ISR244());
void asm_functions(ISR245());
void asm_functions(ISR246());
void asm_functions(ISR247());
void asm_functions(ISR248());
void asm_functions(ISR249());
void asm_functions(ISR250());
void asm_functions(ISR251());
void asm_functions(ISR252());
void asm_functions(ISR253());
void asm_functions(ISR254());
void asm_functions(ISR255());

void asm_functions(IRQ0());
void asm_functions(IRQ1());
void asm_functions(IRQ2());
void asm_functions(IRQ3());
void asm_functions(IRQ4());
void asm_functions(IRQ5());
void asm_functions(IRQ6());
void asm_functions(IRQ7());
void asm_functions(IRQ8());
void asm_functions(IRQ9());
void asm_functions(IRQ10());
void asm_functions(IRQ11());
void asm_functions(IRQ12());
void asm_functions(IRQ13());
void asm_functions(IRQ14());
void asm_functions(IRQ15());

void asm_functions(stop_system());
void asm_functions(syscall_ISR());

void stopping_system();
void test(int t, registers_t* regs);
void isr_init();
void irq_init();
void enable_interrupts();
void isr_register(int interrupt, ISRHandler handler);
void irq_register(int interrupt, ISRHandler handler);
void ISR_setup_static(uint32_t address);

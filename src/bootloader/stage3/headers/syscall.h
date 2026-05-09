#pragma once
#include "isr.h"

#define SYS_OPEN    1
#define SYS_CLOSE   2
#define SYS_WRITE   3

void syscall_init();
void syscall_dispatch(registers_t *regs);
int32_t sys_write(uint32_t fd, const char* buffer, uint32_t count);

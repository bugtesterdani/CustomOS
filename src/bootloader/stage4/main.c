
void make_syscall_test();
void start_paging();

void main()
{
    start_paging();
    make_syscall_test();
}

void make_syscall_test()
{
    unsigned short syscall_addr = 0x10;
    __asm__ __volatile__("movl $0, %eax");
    __asm__ __volatile__("int $0x10");// : : "Nd"(syscall_addr));
}

void start_paging()
{
    __asm__ __volatile__("mov %cr0, %eax");
    __asm__ __volatile__("or 0x80000001, %eax");
    __asm__ __volatile__("mov %eax, %cr0");
}


void make_syscall_test();
void start_paging();
typedef void func(char *, unsigned char, unsigned char);
typedef void func2(void);

void main()
{
    func* printString = (func*)(0x97a2);
    func2* setccurosrnewline = (func2*)(0x91cb);
    printString("Hello world stage 4", 0xf, 0x0);
    setccurosrnewline();
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

#ifndef SYSCALL_H
#define SYSCALL_H

#define SYS_OPEN  1
#define SYS_CLOSE 2
#define SYS_WRITE 3

int open(const char* filename, int flags);
int close(int fd);
int write(int fd, const char* buffer, size_t count);

#endif // SYSCALL_H
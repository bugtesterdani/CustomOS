#pragma once

#define SYS_WRITE   3

int write(int fd, const char* buffer, unsigned int count);

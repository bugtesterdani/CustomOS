#include <iostream>
#include "log/log.h"

void LOG::Error(const char* msg) {
    printf("\x1B[31m");
    printf("%s",msg);
    printf("\033[0m\t\t\n");
}

void LOG::Warning(const char* msg) {
    printf("\x1B[33m");
    printf("%s",msg);
    printf("\033[0m\t\t\n");
}

void LOG::Info(const char* msg) {
    printf("\x1B[32m");
    printf("%s",msg);
    printf("\033[0m\t\t\n");
}

void LOG::Log(const char* msg) {
    printf("%s",msg);
    printf("\n");
}

void LOG::Debug(const char* msg) {
#ifdef DEBUG
    printf("\x1B[34m");
    printf("%s",msg);
    printf("\033[0m\t\t\n");
#else
    (void)msg;
#endif
}
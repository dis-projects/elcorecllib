// Copyright 2020 RnD Center "ELVEES", JSC

#include <errno.h>

#undef errno
extern int errno;

int kill(int pid, int sig) {
    errno = ENOSYS;
    asm("TRAP 10");
    asm("NOP");
    return -1;
}

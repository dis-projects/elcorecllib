// Copyright 2020 RnD Center "ELVEES", JSC

#include <errno.h>

#undef errno
extern int errno;

int fork() {
    errno = ENOSYS;
    asm("TRAP 10");
    asm("NOP");
    return -1;
}

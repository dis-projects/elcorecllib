// Copyright 2020 RnD Center "ELVEES", JSC

#include <errno.h>

#undef errno
extern int errno;

void _exit(int rc) {
    errno = ENOSYS;
    asm("TRAP 10");
    asm("NOP");
}

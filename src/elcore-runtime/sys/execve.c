// Copyright 2020 RnD Center "ELVEES", JSC

#include <errno.h>

#undef errno
extern int errno;

int execve(char *name, char **argv, char **env) {
    errno = ENOSYS;
    asm("TRAP 10");
    asm("NOP");
    return -1;
}

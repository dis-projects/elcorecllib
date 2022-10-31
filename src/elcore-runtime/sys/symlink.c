// Copyright 2020 RnD Center "ELVEES", JSC

#include <errno.h>

#undef errno
extern int errno;

int symlink(const char *path1, const char *path2) {
    errno = ENOSYS;
    asm("TRAP 10");
    asm("NOP");
    return -1;
}

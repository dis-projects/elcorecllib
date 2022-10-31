// Copyright 2020 RnD Center "ELVEES", JSC

#include <errno.h>

#undef errno
extern int errno;

int readlink(const char *path, char *buf, size_t bufsize) {
    errno = ENOSYS;
    asm("TRAP 10");
    asm("NOP");
    return -1;
}

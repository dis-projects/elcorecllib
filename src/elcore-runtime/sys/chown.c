// Copyright 2020 RnD Center "ELVEES", JSC

#include <errno.h>
#include <sys/types.h>

#undef errno
extern int errno;

int chown(const char *path, uid_t owner, gid_t group) {
    errno = ENOSYS;
    asm("TRAP 10");
    asm("NOP");
    return -1;
}

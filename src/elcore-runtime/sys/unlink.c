// Copyright 2020 RnD Center "ELVEES", JSC

#include <errno.h>

#undef errno
extern int errno;

int _syscall_unlink(char *name);

int unlink(char *name) {
    if (name == NULL) {
        errno = EFAULT;
        return -1;
    }
    asm volatile("TRL1 %0, R0" ::"r"(strlen(name)));
    int ret = _syscall_unlink(name);
    if (ret < 0) {
        errno = -ret;
        ret = -1;
    } else
        errno = 0;
    return ret;
}

// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2020 RnD Center "ELVEES", JSC
 */

__attribute__((naked)) void _syscall_gettimeofday() {  // (struct timeval *tv, struct timezone *tz)
    // flush caches
    asm volatile("cchinvr 0x2FC, R0");  // 5 cache lines
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("mbar 0");
    // duplication is required for rf#13078, rf#10191
    asm("TRP 0, P1    SYSCALL 1");
    asm("SYSCALL.P1 1");  // always false
    asm("NOP");
    asm("RTS R15.L");
}

__attribute__((naked)) void _syscall_write() {  // (int fd, void *buf, size_t cout)
    // flush caches
    asm volatile("cchinv 0xFFFC, 0");  // all cache lines
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("mbar 0");
    // duplication is required for rf#13078, rf#10191
    asm("TRP 0, P1    SYSCALL 2");
    asm("SYSCALL.P1 2");  // always false
    asm("NOP");
    asm("RTS R15.L");
}

__attribute__((naked)) void _syscall_read() {  // (int fd, void *buf, size_t cout)
    // flush caches
    asm volatile("cchinv 0xFFFC, 0");  // all cache lines
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("mbar 0");
    // duplication is required for rf#13078, rf#10191
    asm("TRP 0, P1    SYSCALL 3");
    asm("SYSCALL.P1 3");  // always false
    asm("NOP");
    asm("RTS R15.L");
}

__attribute__((naked)) void _syscall_open() {  // (char *filename, int flags, int mode)
    // flush caches
    asm volatile("cchinv 0xFFFC, 0");  // all cache lines
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("mbar 0");
    // duplication is required for rf#13078, rf#10191
    asm("TRP 0, P1    SYSCALL 4");
    asm("SYSCALL.P1 4");  // always false
    asm("NOP");
    asm("RTS R15.L");
}

__attribute__((naked)) void _syscall_close() {  // (int file)
    // duplication is required for rf#13078, rf#10191
    asm("TRP 0, P1    SYSCALL 5");
    asm("SYSCALL.P1 5");  // always false
    asm("NOP");
    asm("RTS R15.L");
}

__attribute__((naked)) void _syscall_fstat() {  // (int file, struct stat *st)
    // flush caches
    asm volatile("cchinvr 0x2FC, R1");  // 5 cache lines
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("mbar 0");
    // duplication is required for rf#13078, rf#10191
    asm("TRP 0, P1    SYSCALL 6");
    asm("SYSCALL.P1 6");  // always false
    asm("NOP");
    asm("RTS R15.L");
}

__attribute__((naked)) void _syscall_lseek() {  // (int file, int offset, int dir)
    // duplication is required for rf#13078, rf#10191
    asm("TRP 0, P1    SYSCALL 7");
    asm("SYSCALL.P1 7");  // always false
    asm("NOP");
    asm("RTS R15.L");
}

__attribute__((naked)) void _syscall_isatty() {  // (int file)
    // duplication is required for rf#13078, rf#10191
    asm("TRP 0, P1    SYSCALL 8");
    asm("SYSCALL.P1 8");  // always false
    asm("NOP");
    asm("RTS R15.L");
}

__attribute__((naked)) void _syscall_chdir() {  // (const char *path)
    // flush caches
    asm volatile("cchinv 0xFFFC, 0");  // all cache lines
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("mbar 0");
    // duplication is required for rf#13078, rf#10191
    asm("TRP 0, P1    SYSCALL 9");
    asm("SYSCALL.P1 9");  // always false
    asm("NOP");
    asm("RTS R15.L");
}

__attribute__((naked)) void _syscall_stat() {  // (const char *filename, struct stat *buf)
    // flush caches
    asm volatile("cchinv 0xFFFC, 0");  // all cache lines
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("mbar 0");
    // duplication is required for rf#13078, rf#10191
    asm("TRP 0, P1    SYSCALL 10");
    asm("SYSCALL.P1 10");  // always false
    asm("NOP");
    asm("RTS R15.L");
}

__attribute__((naked)) void _syscall_times() {  // (struct tms *buf)
    // flush caches
    asm volatile("cchinvr 0x2FC, R0");  // 5 cache lines
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("mbar 0");
    // duplication is required for rf#13078, rf#10191
    asm("TRP 0, P1    SYSCALL 11");
    asm("SYSCALL.P1 11");  // always false
    asm("NOP");
    asm("RTS R15.L");
}

__attribute__((naked)) void _syscall_link() {  // (const char *oldpath, const char *newpath)
    // flush caches
    asm volatile("cchinv 0xFFFC, 0");  // all cache lines
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("mbar 0");
    // duplication is required for rf#13078, rf#10191
    asm("TRP 0, P1    SYSCALL 12");
    asm("SYSCALL.P1 12");  // always false
    asm("NOP");
    asm("RTS R15.L");
}

__attribute__((naked)) void _syscall_unlink() {  // (const char *pathname)
    // flush caches
    asm volatile("cchinv 0xFFFC, 0");  // all cache lines
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("mbar 0");
    // duplication is required for rf#13078, rf#10191
    asm("TRP 0, P1    SYSCALL 13");
    asm("SYSCALL.P1 13");  // always false
    asm("NOP");
    asm("RTS R15.L");
}

__attribute__((naked)) void _syscall_get_env() {  // (char *, uint32_t *)
    // flush caches
    asm volatile("cchinv 0xFFFC, 0");  // all cache lines
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("mbar 0");
    // duplication is required for rf#13078, rf#10191
    asm("TRP 0, P1    SYSCALL 100");
    asm("SYSCALL.P1 100");  // always false
    asm("NOP");
    asm("RTS R15.L");
}

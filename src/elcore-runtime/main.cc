// Copyright 2019-2021 RnD Center "ELVEES", JSC

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

extern "C" void _exit(int rc);
extern "C" int chdir(const char *path);
extern "C" int chown(const char *path, uid_t owner, gid_t group);
extern "C" int close(int fildes);
extern "C" int execve(char *name, char **argv, char **env);
extern "C" int fork();
extern "C" int fstat(int fildes, struct stat *st);
extern "C" int getpid();
extern "C" int gettimeofday(struct timeval *ptimeval, void *ptimezone);
extern "C" int isatty(int file);
extern "C" int kill(int pid, int sig);
extern "C" int link(char *oldpath, char *newpath);
extern "C" int lseek(int file, int ptr, int dir);
extern "C" int open(char *file, int flags, int mode);
extern "C" int read(int file, char *ptr, int len);
extern "C" int readlink(const char *path, char *buf, size_t bufsize);
extern "C" void *sbrk(int incr);
extern "C" int stat(const char  *file, struct stat *st);
extern "C" int symlink(const char *path1, const char *path2);
extern "C" int times(struct tms *buf);
extern "C" int unlink(char *name);
extern "C" int wait(int *status);
extern "C" int write(int file, char *ptr, int len);

// For internal use only
extern "C" int _syscall_get_env(char *env, void *size);

int main(int32_t argc, char *argv[]) __attribute__((weak)) { return 0; }
extern "C" int main_with_share_mem(int32_t argc, char *argv[], char *shmem_ptr,
                                   int32_t shmem_size) __attribute__((weak)) { return 0; }

typedef void (*func_ptr)(void);
extern func_ptr __CTOR_LIST__[];
extern func_ptr __CTOR_END__[];

extern "C" void _init(void) {
    static func_ptr *p = __CTOR_END__;
    while (p > __CTOR_LIST__) {
        (*(--p))();
    }
}

extern "C" int __elcore_before_main() {
    asm("STOP");
    asm("NOP");
    // workaround for rf#11950, rf#11949 and rf#11943
    _exit(0);
    chdir(0);
    chown(0, 0, 0);
    close(0);
    execve(0, 0, 0);
    fork();
    fstat(0, 0);
    getpid();
    gettimeofday(0, 0);
    isatty(0);
    kill(0, 0);
    link(0, 0);
    lseek(0, 0, 0);
    open(0, 0, 0);
    read(0, 0, 0);
    readlink(0, 0, 0);
    sbrk(0);
    stat(0, 0);
    symlink(0, 0);
    times(0);
    unlink(0);
    wait(0);
    write(0, 0, 0);
    return 0;
}

void parse_args(char *args, int& argc,  char **&argv) {
    // Form argc & argv
    size_t arg_size;
    argc = 0;
    argv = NULL;
    while (arg_size = strlen(args)) {
        argc++;
        argv = (char **)realloc(argv, argc * sizeof(char*));
        argv[argc - 1] = args;
        args += arg_size + 1;
    }
}

extern "C" void elcore_main_wrapper(char *args, uint32_t *retval) {
    int argc; char **argv;
    parse_args(args, argc, argv);
    *retval = main(argc, argv);

    free(argv);
}

// Use in elcorecl-run utility from lib/elcorecl-extra
extern "C" void elcorecl_run_wrapper(char* args, uint32_t *retval, char *shared_mem,
                                     uint32_t shared_mem_size) {
    int argc; char **argv;
    parse_args(args, argc, argv);
    *retval = main_with_share_mem(argc, argv, shared_mem, shared_mem_size);

    free(argv);
}

extern "C" void initialize_job(void) {
    // TODO: Code to initialize job
}

extern "C" void deinitialize_job(void) {
    // TODO: Code to deinitialize job
}

extern "C" __attribute__((naked)) void elcore_job_launcher(void) {
    asm(
        // save registers
        "TRL R0.L, R24.L\n\t" // arg0
        "TRL R1.L, R25.L\n\t" // arg1
        "TRL R2.L, R26.L\n\t" // arg2
        "TRL R3.L, R27.L\n\t" // arg3

        "TRL _initialize_job, R9.L\n\t"
        "JS R9.L, R15.l\n\t"

        // restore registers
        "TRL R27.L, R3.L\n\t" // arg3
        "TRL R26.L, R2.L\n\t" // arg2
        "TRL R25.L, R1.L\n\t" // arg1
        "TRL R24.L, R0.L\n\t" // arg0

        "JS R8.L, R15.L\n\t" // job

        "TRL R0.L, R24.L\n\t" // save return value

        "TRL _deinitialize_job, R9.l\n\t"
        "JS R9.L, R15.l\n\t"

        "TRL R24.L, R0.L\n\t" // restore return value
        "STOP\n\t");
}

extern "C" void __get_elcore_argv(void) { }

extern "C" void __init_elcore_environ(void) {
    char *host_env = nullptr;
    uint32_t host_env_size = 0;

    int ret = _syscall_get_env(host_env, &host_env_size);
    if (ret < 0) exit(-1);
    if (host_env_size == 0) exit(-1);

    host_env = (char *) malloc(host_env_size);
    if (!host_env) exit(-1);
    ret = _syscall_get_env(host_env, &host_env_size);
    if (ret < 0) {
        free(host_env);
        exit(-1);
    }

    size_t sz;
    char *env_name, *env_val;
    char *tmp = host_env;
    while ((sz = strlen(tmp))) {
        env_name = tmp;
        tmp += sz + 1;

        sz = strlen(tmp);
        env_val = tmp;
        tmp += sz + 1;
        setenv(env_name, env_val, 0);
    }
    free(host_env);
}

extern "C" int get_core_id(void) {
    uint32_t idr_value;

    asm volatile("TCR IDR, %0" : "=r"(idr_value) :);

    return (idr_value & 0xF) + ((idr_value >> 4) & 0xF) * ((idr_value >> 8) & 0xF);
}

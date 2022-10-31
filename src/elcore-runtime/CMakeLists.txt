cmake_minimum_required(VERSION 3.10)

project(elcore-runtime C CXX ASM)

set(SYSCALL_SRC
    sys/chdir.c
    sys/chown.c
    sys/close.c
    sys/errno.c
    sys/execve.c
    sys/fork.c
    sys/fstat.c
    sys/getpid.c
    sys/gettimeofday.c
    sys/isatty.c
    sys/kill.c
    sys/link.c
    sys/lseek.c
    sys/open.c
    sys/read.c
    sys/readlink.c
    sys/sbrk.c
    sys/stat.c
    sys/symlink.c
    sys/syscall-impl.c
    sys/times.c
    sys/unlink.c
    sys/wait.c
    sys/write.c
    sys/_exit.c
)

set(HEADER
    include/dma.h
    include/main.h
    include/syscall-runtime.h
)

add_library(elcore-runtime-api STATIC main.cc dma.cc ${SYSCALL_SRC})
if(BUILD_FOR_MCOM03)
    target_compile_definitions(elcore-runtime-api PUBLIC -DBUILD_FOR_MCOM03)
endif()
set_target_properties(elcore-runtime-api PROPERTIES PUBLIC_HEADER "${HEADER}")

target_include_directories(elcore-runtime-api PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:lib/elcore/runtime/include>)

add_library(elcore-runtime INTERFACE)
# Disable pyapi.a system calls #SOLARIS-3634
target_link_options(elcore-runtime INTERFACE -mno-pyapi)

target_link_libraries(elcore-runtime INTERFACE
                      -Wl,-whole-archive elcore-runtime-api -Wl,-no-whole-archive)

set_target_properties(elcore-runtime PROPERTIES PUBLIC_HEADER "${HEADER}")
target_include_directories(elcore-runtime INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:lib/elcore/runtime/include>)

install(TARGETS elcore-runtime elcore-runtime-api EXPORT elcore-runtime
        ARCHIVE DESTINATION lib/elcore/runtime/lib
        PUBLIC_HEADER DESTINATION lib/elcore/runtime/include)
install(EXPORT elcore-runtime FILE elcoreruntime-config.cmake
        DESTINATION lib/cmake/ElcoreRuntime)

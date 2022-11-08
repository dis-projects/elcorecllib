cmake_minimum_required(VERSION 3.12)

project(Risc1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
Set(CMAKE_BUILD_TYPE Debug)

add_library(risc1 SHARED syscall.cc job-wrapper.cc)

set_target_properties(risc1 PROPERTIES PUBLIC_HEADER librisc1.h)
if(BUILD_FOR_MCOM03)
    target_compile_definitions(risc1 PRIVATE -DBUILD_FOR_MCOM03)
endif()
target_include_directories(risc1
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/../include>
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
      $<INSTALL_INTERFACE:include>)

install(TARGETS risc1
        EXPORT risc1
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include)


cmake_minimum_required(VERSION 3.12)

project(Elcore)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(elcore SHARED syscall.cc job-wrapper.cc)

set_target_properties(elcore PROPERTIES PUBLIC_HEADER libelcore.h)
if(BUILD_FOR_MCOM03)
    target_compile_definitions(elcore PRIVATE -DBUILD_FOR_MCOM03)
endif()
target_include_directories(elcore
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/../include>
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
      $<INSTALL_INTERFACE:include>)

install(TARGETS elcore
        EXPORT elcore
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include)
install(EXPORT elcore
        FILE ElcoreConfig.cmake
        DESTINATION lib/cmake/Elcore)

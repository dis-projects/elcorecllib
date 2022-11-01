cmake_minimum_required(VERSION 3.19)

project(elcorecllib)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_FOR_MCOM03 TRUE)
set(ELCORE_CMAKE_TOOLCHAIN_FILE "/opt/eltools_4.0_linux/share/cmake/elcore50_toolchain.cmake")

include (elcorecl/ElcoreProject.cmake)

elcoreproject_add(ELCORE_RUNTIME elcore-runtime
    CMAKE_ARGS "-DCMAKE_CXX_FLAGS=-O3" "-DCMAKE_C_FLAGS=-O3"
    -DBUILD_FOR_MCOM03=${BUILD_FOR_MCOM03})
install(DIRECTORY ${ELCORE_RUNTIME_INSTALL_DIR}/lib DESTINATION ${CMAKE_INSTALL_PREFIX})

add_subdirectory(elcore)
add_subdirectory(elcorecl)

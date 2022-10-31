elcoreproject_add(ELCORE_RUNTIME elcore-runtime
    CMAKE_ARGS "-DCMAKE_CXX_FLAGS=-O3" "-DCMAKE_C_FLAGS=-O3"
    -DBUILD_FOR_MCOM03=${BUILD_FOR_MCOM03})
install(DIRECTORY ${ELCORE_RUNTIME_INSTALL_DIR}/lib DESTINATION ${CMAKE_INSTALL_PREFIX})

add_subdirectory(elcore)
add_subdirectory(elcorecl)

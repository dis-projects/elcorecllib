set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)

set(BUILD_SHARED_LIBS TRUE)

if(${BUILD_SHARED_LIBS})
    set(Boost_USE_STATIC_LIBS OFF)
else()
    set(Boost_USE_STATIC_LIBS ON)
endif()
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS filesystem regex)

include(CMakePackageConfigHelpers)  # for configure_package_config_file

set(HEADER_PATH "${PROJECT_SOURCE_DIR}/include/elcorecl")
set(HEADER
    ${HEADER_PATH}/ecl.h
    ${HEADER_PATH}/ecl_d3d10.h
    ${HEADER_PATH}/ecl_d3d11.h
    ${HEADER_PATH}/ecl_dx9_media_sharing.h
    ${HEADER_PATH}/ecl_dx9_media_sharing_intel.h
    ${HEADER_PATH}/ecl_egl.h
    ${HEADER_PATH}/ecl_ext.h
    ${HEADER_PATH}/ecl_ext_intel.h
    ${HEADER_PATH}/ecl_gl.h
    ${HEADER_PATH}/ecl_gl_ext.h
    ${HEADER_PATH}/ecl_platform.h
    ${HEADER_PATH}/ecl_va_api_media_sharing_intel.h
    ${HEADER_PATH}/ecl_version.h
    ${HEADER_PATH}/elcorecl.h)

set(SRC
    command-queue.cc
    context.cc
    device.cc
    enqueued-commands.cc
    event-object.cc
    kernel-object.cc
    memory-object.cc
    object.cc
    platform.cc
    program-object.cc)

if(${BUILD_SHARED_LIBS})
    add_library(elcorecl SHARED ${HEADER} ${SRC})
else()
    add_library(elcorecl STATIC ${HEADER} ${SRC})
endif()

set_target_properties(elcorecl PROPERTIES PUBLIC_HEADER "${HEADER}")
target_link_libraries(elcorecl PRIVATE Boost::boost Boost::filesystem Boost::regex Threads::Threads
                      elcore)
if(BUILD_FOR_MCOM03)
    target_compile_definitions(elcorecl PRIVATE -DBUILD_FOR_MCOM03)
endif()
target_include_directories(elcorecl
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/../../include>
      $<BUILD_INTERFACE:${ELCORE_SRC_DIR}>
      $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>
      $<INSTALL_INTERFACE:include>)
if(NOT ${ELCORE_SRC_DIR} STREQUAL "")
    target_compile_definitions(elcorecl PUBLIC $<BUILD_INTERFACE:DRIVER_NOT_INSTALLED>)
endif()

install(TARGETS elcorecl EXPORT elcorecl
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION ../../include/elcorecl)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/elcorecl-config.cmake
    INSTALL_DESTINATION lib/cmake/ElcoreCL)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/elcorecl-config.cmake
              ${CMAKE_CURRENT_SOURCE_DIR}/ElcoreProject.cmake
        DESTINATION lib/cmake/ElcoreCL)

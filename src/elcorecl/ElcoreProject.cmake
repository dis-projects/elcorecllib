# Copyright 2019-2022 RnD Center "ELVEES", JSC
# SPDX-License-Identifier: BSD-3-Clause
#
#.rst:
# ElcoreProject
# -------------
#
# Helper function for building Elcore targets as external projects.
#
# ``ELCORE_CMAKE_TOOLCHAIN_FILE`` should point to CMake toolchain file for Elcore.
#
#
# This module defines the following function:
#
# .. command:: elcoreproject_add
#
#   The ``elcoreproject_add()`` creates ExternalProject that will be built with Elcore toolchain.
#
#   elcoreproject_add(<name> <source_dir> [<option>...])
#
#   **Required Arguments:**
#     ``<name>``
#       Name of external project to add. ``<name>`` can be used functions from ExternalProject
#       module like ``ExternalProject_Get_Property`` or as dependency for targets.
#
#     ``<source_dir>``
#       Path to directory containing sources of the project to build. The path may be absolute or
#       relative to ``CMAKE_CURRENT_SOURCE_DIR``.
#
#   **Optional Arguments:**
#     ``CMAKE_ARGS <args>``
#       Arguments passed to the subproject.
#
#   ``elcoreproject_add()`` sets variable ``<name>_INSTALL_DIR`` to installation path of
#   subproject.

include(ExternalProject)
if(NOT EXISTS ${ELCORE_CMAKE_TOOLCHAIN_FILE})
    message(SEND_ERROR "CMake toolchain file not found. Following path was specified: \
            '${ELCORE_CMAKE_TOOLCHAIN_FILE}'")
endif()

function(elcoreproject_add name source_dir)
    if(NOT IS_ABSOLUTE ${source_dir})
        set(source_dir ${CMAKE_CURRENT_SOURCE_DIR}/${source_dir})
    endif()
    cmake_parse_arguments(ELCORE_PROJECT "" "" "CMAKE_ARGS" ${ARGN})
    externalproject_add(${name}
                        SOURCE_DIR ${source_dir}
                        CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${ELCORE_CMAKE_TOOLCHAIN_FILE}
                                    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                                    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
                                    ${ELCORE_PROJECT_CMAKE_ARGS}
                        TEST_EXCLUDE_FROM_MAIN TRUE
                        BUILD_ALWAYS TRUE)
    externalproject_get_property(${name} INSTALL_DIR)
    set(${name}_INSTALL_DIR ${INSTALL_DIR} PARENT_SCOPE)
endfunction()

# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.15)

set(CMAKE_BUILD_TYPE_INIT Release)

project(sbl)

set(GIT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "Git DIR")

execute_process(
    COMMAND git describe --always --dirty
    WORKING_DIRECTORY ${GIT_DIR}
    ERROR_QUIET
    OUTPUT_VARIABLE COMMIT)
if ("${COMMIT}" STREQUAL "")
    set(COMMIT "unknown")
endif()

string(STRIP "${COMMIT}" COMMIT)

add_compile_definitions(COMMIT="${COMMIT}")

if (NOT "$ENV{BUILD_ID}" STREQUAL "")
    add_compile_definitions(BUILD_ID="$ENV{BUILD_ID}")
endif()

add_subdirectory(common)
add_subdirectory(sbl-s1)
add_subdirectory(sbl-xip)
add_subdirectory(scripts)

# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.15)

set(CMAKE_BUILD_TYPE_INIT Release)

project(sbl ASM C)

set(DDRINIT_DTB_MAP_FILE CACHE PATH "ddrinit-DTB map file")
if(NOT EXISTS ${DDRINIT_DTB_MAP_FILE})
    message(FATAL_ERROR "Map file ${DDRINIT_DTB_MAP_FILE} doesn't exist")
endif()

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

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_options(-gdwarf-2 -fno-eliminate-unused-debug-types
                        -g -O0)
    add_compile_definitions(LOG_LEVEL=40)
else()
    add_compile_options(-g0 -O2)
    add_compile_definitions(LOG_LEVEL=20 NDEBUG)
endif()

add_compile_options(-Wall -Werror -std=gnu99 -EL -mips32 -G0
                    -mabi=32 -mlong32 -mno-gpopt -mlong-calls -mllsc
                    -ffunction-sections -fdata-sections -ffreestanding
                    -fno-builtin -fno-delayed-branch
                    -ftrivial-auto-var-init=zero)

add_link_options(-Wl,--gc-sections -Wl,--print-memory-usage
                 -EL -nostdlib -nostartfiles -mips32 -mabi=32 -G0)

add_subdirectory(common)
add_subdirectory(sbl-s1)
add_subdirectory(sbl-xip)
add_subdirectory(scripts)

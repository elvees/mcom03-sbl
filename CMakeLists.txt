# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

set(CMAKE_BUILD_TYPE_INIT Release)

project(sbl ASM C)

include(cmake/helpers.cmake)

set(BUILD_UNIT_TESTS OFF CACHE BOOL "Enable building unit tests")
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

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/third-party
                    ${CMAKE_CURRENT_SOURCE_DIR}/third-party/libfdt)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_options(-gdwarf-2 -fno-eliminate-unused-debug-types -g -O0)
else()
    add_compile_options(-g0 -O2)
    add_compile_definitions(NDEBUG)
endif()

add_compile_options(-ffunction-sections -fdata-sections)

add_link_options(-Wl,--gc-sections)

if(BUILD_UNIT_TESTS)
    add_subdirectory(tests)
else()
    set(DDRINIT_DTB_MAP_FILE CACHE PATH "ddrinit-DTB map file")
    if(NOT EXISTS ${DDRINIT_DTB_MAP_FILE})
        message(FATAL_ERROR "DTB Map file ${DDRINIT_DTB_MAP_FILE} doesn't exist")
    endif()

    set(WDT_ENABLE TRUE CACHE BOOL "Enable WDT")
    set(UART_ENABLE TRUE CACHE BOOL "Enable UART")
    set(DEBUG_INTERFACE_DISABLE OFF CACHE BOOL "Disable SoC debug interface")
    set(RECOVERY_ENABLE FALSE CACHE BOOL "Enable recovery support")

    if(CMAKE_BUILD_TYPE MATCHES Debug)
        add_compile_definitions(LOG_LEVEL=40)
    else()
        add_compile_definitions(LOG_LEVEL=20)
        if(WDT_ENABLE)
            add_compile_definitions(WDT_ENABLE)
        endif()

        if(DEBUG_INTERFACE_DISABLE)
            add_compile_definitions(DEBUG_INTERFACE_DISABLE)
        endif()
    endif()

    add_compile_options(-Wall -Werror -std=gnu99 -EL -mips32 -G0
                        -mabi=32 -mlong32 -mno-gpopt -mlong-calls -mllsc
                        -ffreestanding -fno-builtin -fno-delayed-branch
                        -fstack-protector-all -ftrivial-auto-var-init=zero)

    add_link_options(-Wl,--print-memory-usage -Wl,--wrap=__assert_func
                     -EL -nostdlib -nostartfiles -mips32 -mabi=32 -G0)

    if(UART_ENABLE)
        add_compile_definitions(UART_ENABLE)
    endif()

    if(RECOVERY_ENABLE)
        add_compile_definitions(RECOVERY_ENABLE)
    endif()

    add_subdirectory(third-party)
    add_subdirectory(drivers)
    add_subdirectory(libs)

    add_library(common STATIC
                $<TARGET_OBJECTS:third-party>
                $<TARGET_OBJECTS:drivers>
                $<TARGET_OBJECTS:libs>)

    add_subdirectory(sbl-s1)
    add_subdirectory(sbl-s2)
    add_subdirectory(sbl-s3)
    add_subdirectory(sbl-xip)
    add_subdirectory(scripts)
endif()

# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

set(CMAKE_BUILD_TYPE_INIT Release)

project(sbl ASM C)

include(cmake/helpers.cmake)

set(BUILD_TARGET DEFAULT CACHE STRING
    "Build target. Values: UNIT_TESTS, COMMON_LIBS otherwise SBL will be built")
set(GIT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "Git DIR")
set(BUILD_ID CACHE STRING "Build identificator")
set(IMG_ALIGN 16 CACHE STRING "Image alignment")

execute_process(
    COMMAND git describe --always --dirty
    WORKING_DIRECTORY ${GIT_DIR}
    ERROR_QUIET
    OUTPUT_VARIABLE BUILD_COMMIT)
if("${BUILD_COMMIT}" STREQUAL "")
    set(BUILD_COMMIT "unknown")
endif()

string(STRIP "${BUILD_COMMIT}" BUILD_COMMIT)

add_compile_definitions(BUILD_COMMIT="${BUILD_COMMIT}")

if(NOT "${BUILD_ID}" STREQUAL "")
    add_compile_definitions(BUILD_ID="${BUILD_ID}")
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

set(MipsCSP_LIBRARIES mips-csp)
set(MipsCSP_INCLUDE_DIRS third-party drivers libs)
include(cmake/MipsCSPConfig.cmake)

if("${BUILD_TARGET}" STREQUAL "UNIT_TESTS")
    add_subdirectory(tests)
elseif("${BUILD_TARGET}" STREQUAL "COMMON_LIBS")
    build_static_library(MipsCSP_INCLUDE_DIRS ${MipsCSP_LIBRARIES})
else()
    set(DDRINIT_DTB_MAP_FILE CACHE PATH "ddrinit-DTB map file")
    if(NOT EXISTS ${DDRINIT_DTB_MAP_FILE})
        message(FATAL_ERROR "DTB Map file ${DDRINIT_DTB_MAP_FILE} doesn't exist")
    endif()

    set(WDT_ENABLE TRUE CACHE BOOL "Enable WDT")
    set(UART_ENABLE TRUE CACHE BOOL "Enable UART")
    set(RECOVERY_ENABLE FALSE CACHE BOOL "Enable recovery support")
    set(BOOTSTAGE_ENABLE TRUE CACHE BOOL "Enable bootstage support")

    if(CMAKE_BUILD_TYPE MATCHES Debug)
        add_compile_definitions(LOG_LEVEL=40)
    else()
        add_compile_definitions(LOG_LEVEL=20)
        if(WDT_ENABLE)
            add_compile_definitions(WDT_ENABLE)
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

    if(BOOTSTAGE_ENABLE)
        add_compile_definitions(BOOTSTAGE_ENABLE)
    endif()

    build_static_library(MipsCSP_INCLUDE_DIRS ${MipsCSP_LIBRARIES})

    add_subdirectory(sbl-s1)
    add_subdirectory(sbl-s2)
    add_subdirectory(sbl-s3)
    add_subdirectory(sbl-xip)
    add_subdirectory(scripts)
    add_subdirectory(tl-sbimg)
endif()

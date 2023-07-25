# SPDX-License-Identifier: MIT

project(common ASM C)

add_library(${PROJECT_NAME} STATIC common.c)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(${PROJECT_NAME} PRIVATE
                           -gdwarf-2 -fno-eliminate-unused-debug-types)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -g0)
endif()

target_compile_options(${PROJECT_NAME} PRIVATE
                       -Wall -Wno-nonnull -Werror -std=c99 -EL -mips32 -G0
                       -mabi=32 -fno-builtin-malloc -ffunction-sections
                       -fdata-sections)

target_include_directories(${PROJECT_NAME} PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR})

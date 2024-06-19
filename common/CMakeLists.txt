# SPDX-License-Identifier: MIT

project(common ASM C)

add_library(${PROJECT_NAME} STATIC common.c)

target_include_directories(${PROJECT_NAME} PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR})

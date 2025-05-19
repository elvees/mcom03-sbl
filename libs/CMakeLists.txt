# SPDX-License-Identifier: MIT

add_library(libs OBJECT
            ${CMAKE_CURRENT_SOURCE_DIR}/console/console.c
            ${CMAKE_CURRENT_SOURCE_DIR}/env/env.c
            ${CMAKE_CURRENT_SOURCE_DIR}/env/env-io.c
            ${CMAKE_CURRENT_SOURCE_DIR}/env/env-io-ram.c
            ${CMAKE_CURRENT_SOURCE_DIR}/env/env-io-spi.c
            ${CMAKE_CURRENT_SOURCE_DIR}/fdt-helpers/fdt-helpers.c
            ${CMAKE_CURRENT_SOURCE_DIR}/helpers/helpers.c
            ${CMAKE_CURRENT_SOURCE_DIR}/platform/assert.c
            ${CMAKE_CURRENT_SOURCE_DIR}/platform/format-parser.c
            ${CMAKE_CURRENT_SOURCE_DIR}/platform/printf.c
            ${CMAKE_CURRENT_SOURCE_DIR}/platform/putchar.c
            ${CMAKE_CURRENT_SOURCE_DIR}/platform/sbrk.c
            ${CMAKE_CURRENT_SOURCE_DIR}/platform/snprintf.c
            ${CMAKE_CURRENT_SOURCE_DIR}/platform/stack-protector.c
            ${CMAKE_CURRENT_SOURCE_DIR}/queue/queue.c)

# SPDX-License-Identifier: MIT

add_library(third-party OBJECT
            ${CMAKE_CURRENT_SOURCE_DIR}/crc/crc8.c
            ${CMAKE_CURRENT_SOURCE_DIR}/crc/crc16.c
            ${CMAKE_CURRENT_SOURCE_DIR}/crc/crc32.c
            ${CMAKE_CURRENT_SOURCE_DIR}/crc/crc64.c
            ${CMAKE_CURRENT_SOURCE_DIR}/crc/crcccitt.c
            ${CMAKE_CURRENT_SOURCE_DIR}/crc/crcdnp.c
            ${CMAKE_CURRENT_SOURCE_DIR}/crc/crckrmit.c
            ${CMAKE_CURRENT_SOURCE_DIR}/crc/crcsick.c
            ${CMAKE_CURRENT_SOURCE_DIR}/crc/nmea-chk.c

            ${CMAKE_CURRENT_SOURCE_DIR}/libfdt/fdt.c
            ${CMAKE_CURRENT_SOURCE_DIR}/libfdt/fdt_addresses.c
            ${CMAKE_CURRENT_SOURCE_DIR}/libfdt/fdt_check.c
            ${CMAKE_CURRENT_SOURCE_DIR}/libfdt/fdt_empty_tree.c
            ${CMAKE_CURRENT_SOURCE_DIR}/libfdt/fdt_overlay.c
            ${CMAKE_CURRENT_SOURCE_DIR}/libfdt/fdt_ro.c
            ${CMAKE_CURRENT_SOURCE_DIR}/libfdt/fdt_rw.c
            ${CMAKE_CURRENT_SOURCE_DIR}/libfdt/fdt_strerror.c
            ${CMAKE_CURRENT_SOURCE_DIR}/libfdt/fdt_sw.c
            ${CMAKE_CURRENT_SOURCE_DIR}/libfdt/fdt_wip.c

            ${CMAKE_CURRENT_SOURCE_DIR}/llist/llist.c)

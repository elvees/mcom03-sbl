# SPDX-License-Identifier: MIT

add_library(drivers OBJECT
            ${CMAKE_CURRENT_SOURCE_DIR}/cpu/cpu.c
            ${CMAKE_CURRENT_SOURCE_DIR}/gpio/gpio.c
            ${CMAKE_CURRENT_SOURCE_DIR}/hs-periph/hs-periph.c
            ${CMAKE_CURRENT_SOURCE_DIR}/ls-periph1/ls-periph1.c
            ${CMAKE_CURRENT_SOURCE_DIR}/pll/pll.c
            ${CMAKE_CURRENT_SOURCE_DIR}/service/service.c
            ${CMAKE_CURRENT_SOURCE_DIR}/top/top.c
            ${CMAKE_CURRENT_SOURCE_DIR}/uart/uart.c
            ${CMAKE_CURRENT_SOURCE_DIR}/uart/uart-config.c
            ${CMAKE_CURRENT_SOURCE_DIR}/ucg/ucg.c
            ${CMAKE_CURRENT_SOURCE_DIR}/wdt/wdt.c
            ${CMAKE_CURRENT_SOURCE_DIR}/wdt/wdt-config.c)

# SPDX-License-Identifier: MIT

add_library(drivers OBJECT
            ${CMAKE_CURRENT_SOURCE_DIR}/cpu/cpu.c
            ${CMAKE_CURRENT_SOURCE_DIR}/gpio/gpio.c
            ${CMAKE_CURRENT_SOURCE_DIR}/hs-periph/hs-periph.c
            ${CMAKE_CURRENT_SOURCE_DIR}/iommu/iommu.c
            ${CMAKE_CURRENT_SOURCE_DIR}/ls-periph1/ls-periph1.c
            ${CMAKE_CURRENT_SOURCE_DIR}/mailbox/mailbox.c
            ${CMAKE_CURRENT_SOURCE_DIR}/mips-cp0/mips-cp0.c
            ${CMAKE_CURRENT_SOURCE_DIR}/pll/pll.c
            ${CMAKE_CURRENT_SOURCE_DIR}/qlic/irq.c
            ${CMAKE_CURRENT_SOURCE_DIR}/qlic/qlic.c
            ${CMAKE_CURRENT_SOURCE_DIR}/qspi/qspi.c
            ${CMAKE_CURRENT_SOURCE_DIR}/service/service.c
            ${CMAKE_CURRENT_SOURCE_DIR}/spi-nor/spi-nor.c
            ${CMAKE_CURRENT_SOURCE_DIR}/timer/timer.c
            ${CMAKE_CURRENT_SOURCE_DIR}/top/top.c
            ${CMAKE_CURRENT_SOURCE_DIR}/uart/uart.c
            ${CMAKE_CURRENT_SOURCE_DIR}/ucg/ucg.c
            ${CMAKE_CURRENT_SOURCE_DIR}/wdt/wdt.c)

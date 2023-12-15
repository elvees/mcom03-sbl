# SPDX-License-Identifier: MIT

project(sbl-s1 ASM C)

set(WDT_ENABLE TRUE CACHE BOOL "WDT is enabled")
set(PRESETUP_UARTS TRUE CACHE BOOL "Pre-setup UART")

# Get common library include path
get_target_property(COMMON_INCLUDE_DIRECTORIES common INCLUDE_DIRECTORIES)

add_executable(${PROJECT_NAME}.elf
               drivers/gpio.c
               drivers/hs-periph-urb.c
               drivers/ls-periph1-urb.c
               drivers/pll.c
               drivers/service-urb.c
               drivers/uart.c
               drivers/ucg.c
               drivers/wdt.c
               library/uart-printf.c
               source/hwinit.c
               source/startup.S)

target_include_directories(${PROJECT_NAME}.elf PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           ${CMAKE_CURRENT_SOURCE_DIR}/include
                           ${COMMON_INCLUDE_DIRECTORIES})

if(PRESETUP_UARTS)
    target_compile_definitions(${PROJECT_NAME}.elf PRIVATE -DPRESETUP_UARTS)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(${PROJECT_NAME}.elf PRIVATE
                           -gdwarf-2 -fno-eliminate-unused-debug-types)
    target_compile_definitions(${PROJECT_NAME}.elf PRIVATE
                               -DENABLE_ASSERTIONS -DDEBUG_IFACE_USE)
else()
    target_compile_options(${PROJECT_NAME}.elf PRIVATE -g0)
    if (WDT_ENABLE)
        target_compile_definitions(${PROJECT_NAME}.elf PRIVATE -DWDT_USE)
    endif()
endif()

target_compile_options(${PROJECT_NAME}.elf PRIVATE
                       -Wall -Wno-nonnull -Werror -std=c99 -EL -mips32 -G0
                       -mabi=32 -fno-builtin-malloc -ffunction-sections
                       -fdata-sections)

target_link_options(${PROJECT_NAME}.elf PRIVATE
                    -T ${CMAKE_CURRENT_BINARY_DIR}/link.ld
                    -EL -nostdlib -mips32 -mabi=32 -G0 -Wl,--gc-sections
                    -Wl,-Map=${PROJECT_NAME}.map -Wl,--print-memory-usage)

add_custom_command(TARGET ${PROJECT_NAME}.elf PRE_LINK COMMAND ${CMAKE_C_COMPILER}
                   $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS)
                   -E ${CMAKE_CURRENT_SOURCE_DIR}/source/link.ld.S -P
                   -o ${CMAKE_CURRENT_BINARY_DIR}/link.ld)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
                   COMMAND ${CMAKE_OBJDUMP}
                   -S ${PROJECT_NAME}.elf > ${PROJECT_NAME}.dis
                   BYPRODUCTS ${PROJECT_NAME}.dis)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
                   COMMAND ${CMAKE_OBJCOPY}
                   -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
                   BYPRODUCTS ${PROJECT_NAME}.bin)

install(TARGETS ${PROJECT_NAME}.elf DESTINATION .)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.bin DESTINATION .)

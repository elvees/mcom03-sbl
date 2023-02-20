# SPDX-License-Identifier: MIT

project(SBL-XIP ASM C)

set(DDRINIT_PATH CACHE STRING "Path to ddrinit binary")
if(NOT DDRINIT_PATH)
    message(FATAL_ERROR "DDRINIT_PATH is not specified")
endif()

set(TFA_PATH CACHE STRING "Path to TF-A binary")
if(NOT TFA_PATH)
    message(FATAL_ERROR "TFA_PATH is not specified")
endif()

set(UBOOT_PATH CACHE STRING "Path to U-Boot binary")
if(NOT UBOOT_PATH)
    message(FATAL_ERROR "UBOOT_PATH is not specified")
endif()

add_compile_options(-Wall -Wno-nonnull -Werror -std=gnu99 -EL -mips32 -G0
                    -mabi=32 -fno-builtin-malloc -ffunction-sections -fdata-sections)
add_link_options(-T ${CMAKE_CURRENT_BINARY_DIR}/link.ld -Wl,--gc-sections
                 -EL -nostdlib -nostartfiles -mips32 -mabi=32 -G0)

if(BL32_PATH)
    add_compile_options(-DHAS_BL32=1)
endif()

add_executable(sbl-xip.elf start.S sbl.c pll.c ucg.c)

set_target_properties(sbl-xip.elf PROPERTIES LINK_FLAGS "-Wl,-Map=sbl-xip.map")

add_custom_command(TARGET sbl-xip.elf PRE_LINK COMMAND ${CMAKE_C_COMPILER}
                   $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS)
                   -E ${CMAKE_CURRENT_SOURCE_DIR}/link.ld.S -P
                   -o ${CMAKE_CURRENT_BINARY_DIR}/link.ld)

add_custom_command(TARGET sbl-xip.elf PRE_BUILD COMMAND ${CMAKE_OBJCOPY} -I binary
                   -O elf32-littlemips -B mips --rename-section .data=.rodata
                   ${DDRINIT_PATH} ${CMAKE_CURRENT_BINARY_DIR}/ddrinit.o)

add_custom_command(TARGET sbl-xip.elf PRE_BUILD COMMAND ${CMAKE_OBJCOPY} -I binary
                   -O elf32-littlemips -B mips --rename-section .data=.rodata
                   ${TFA_PATH} ${CMAKE_CURRENT_BINARY_DIR}/tfa.o)

if(BL32_PATH)
    add_custom_command(TARGET sbl-xip.elf PRE_BUILD COMMAND ${CMAKE_OBJCOPY} -I binary
                       -O elf32-littlemips -B mips --rename-section .data=.rodata
                       ${BL32_PATH} ${CMAKE_CURRENT_BINARY_DIR}/bl32.o)
endif()

add_custom_command(TARGET sbl-xip.elf PRE_BUILD COMMAND ${CMAKE_OBJCOPY} -I binary
                   -O elf32-littlemips -B mips --rename-section .data=.rodata
                   ${UBOOT_PATH} ${CMAKE_CURRENT_BINARY_DIR}/u-boot.o)

add_custom_command(TARGET sbl-xip.elf POST_BUILD COMMAND ${CMAKE_OBJDUMP}
                   -S sbl-xip.elf > sbl-xip.dis BYPRODUCTS sbl-xip.dis)

add_custom_command(TARGET sbl-xip.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
                   -O binary sbl-xip.elf sbl-xip.bin BYPRODUCTS sbl-xip.bin)

install(TARGETS sbl-xip.elf DESTINATION .)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sbl-xip.bin DESTINATION .)
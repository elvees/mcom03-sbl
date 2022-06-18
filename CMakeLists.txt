cmake_minimum_required(VERSION 3.15)

set(CMAKE_BUILD_TYPE_INIT Debug)

project(SBL ASM C)

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
add_link_options(-T ${CMAKE_CURRENT_SOURCE_DIR}/link.ld
                 -EL -nostdlib -nostartfiles -mips32 -mabi=32 -G0)

add_executable(sbl.elf start.S sbl.c)

set_target_properties(sbl.elf PROPERTIES LINK_FLAGS "-Wl,-Map=sbl.map")

add_custom_command(TARGET sbl.elf PRE_BUILD COMMAND ${CMAKE_OBJCOPY} -I binary
                   -O elf32-littlemips -B mips --rename-section .data=.rodata
                   ${DDRINIT_PATH} ${CMAKE_CURRENT_BINARY_DIR}/ddrinit.o)

add_custom_command(TARGET sbl.elf PRE_BUILD COMMAND ${CMAKE_OBJCOPY} -I binary
                   -O elf32-littlemips -B mips --rename-section .data=.rodata
                   ${TFA_PATH} ${CMAKE_CURRENT_BINARY_DIR}/tfa.o)

add_custom_command(TARGET sbl.elf PRE_BUILD COMMAND ${CMAKE_OBJCOPY} -I binary
                   -O elf32-littlemips -B mips --rename-section .data=.rodata
                   ${UBOOT_PATH} ${CMAKE_CURRENT_BINARY_DIR}/u-boot.o)

add_custom_command(TARGET sbl.elf POST_BUILD COMMAND ${CMAKE_OBJDUMP}
                   -S sbl.elf > sbl.dis BYPRODUCTS sbl.dis)

add_custom_command(TARGET sbl.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
                   -O binary sbl.elf sbl.bin BYPRODUCTS sbl.bin)

install(TARGETS sbl.elf DESTINATION .)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sbl.bin DESTINATION .)

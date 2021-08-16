cmake_minimum_required(VERSION 3.15)

set(CMAKE_BUILD_TYPE_INIT Debug)

project(SBL_CPU ASM C)

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

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    add_compile_options(-Wall -Wno-nonnull -Werror -std=c99)
    add_link_options(-T link.ld -nostartfiles -nostdlib)
    set(CONFIG_REGION "xip1")
    set(CONFIG_ARCH "aarch64")
    set(CONFIG_RAM_ADDR "0x00000000")
    set(BINFILE_FORMAT "elf64-littleaarch64")
else()
    add_definitions(-DMIPS32)
    add_compile_options(-Wall -Wno-nonnull -Werror -EL -mips32 -G0
                        -mabi=32 -fno-builtin-malloc -ffunction-sections -fdata-sections)
    add_link_options(-T link.ld -EL -nostdlib -nostartfiles -mips32 -mabi=32 -G0)
    set(CONFIG_REGION "xip0")
    set(CONFIG_ARCH "mips")
    set(CONFIG_RAM_ADDR "0x9fa00000")
    set(BINFILE_FORMAT "elf32-tradlittlemips")
endif()

set(FNAME sbl-${CONFIG_ARCH})

add_executable(${FNAME}.elf start-${CONFIG_ARCH}.S sbl-cpu.c)

set_target_properties(${FNAME}.elf PROPERTIES LINK_FLAGS "-Wl,-Map=${FNAME}.map")

add_custom_command(TARGET ${FNAME}.elf PRE_BUILD
                   COMMAND cp ${PROJECT_SOURCE_DIR}/link.ld.in link.ld
                   COMMAND sed -i 's/CONFIG_REGION/${CONFIG_REGION}/g' link.ld
                   COMMAND sed -i 's/CONFIG_ARCH/${CONFIG_ARCH}/g' link.ld
                   COMMAND sed -i 's/CONFIG_RAM_ADDR/${CONFIG_RAM_ADDR}/g' link.ld
                   BYPRODUCTS link.ld ${FNAME}.map)

add_custom_command(TARGET ${FNAME}.elf PRE_BUILD COMMAND ${CMAKE_OBJCOPY} -I binary
                   -O ${BINFILE_FORMAT} -B ${CONFIG_ARCH} --rename-section .data=.rodata
                   ${DDRINIT_PATH} ${CMAKE_CURRENT_BINARY_DIR}/ddrinit.o)

add_custom_command(TARGET ${FNAME}.elf PRE_BUILD COMMAND ${CMAKE_OBJCOPY} -I binary
                   -O ${BINFILE_FORMAT} -B ${CONFIG_ARCH} --rename-section .data=.rodata
                   ${TFA_PATH} ${CMAKE_CURRENT_BINARY_DIR}/tfa.o)

add_custom_command(TARGET ${FNAME}.elf PRE_BUILD COMMAND ${CMAKE_OBJCOPY} -I binary
                   -O ${BINFILE_FORMAT} -B ${CONFIG_ARCH} --rename-section .data=.rodata
                   ${UBOOT_PATH} ${CMAKE_CURRENT_BINARY_DIR}/u-boot.o)

add_custom_command(TARGET ${FNAME}.elf POST_BUILD COMMAND ${CMAKE_OBJDUMP}
                   -S ${FNAME}.elf > ${FNAME}.dis BYPRODUCTS ${FNAME}.dis)

add_custom_command(TARGET ${FNAME}.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
                   -O binary ${FNAME}.elf ${FNAME}.bin BYPRODUCTS ${FNAME}.bin)

install(TARGETS ${FNAME}.elf DESTINATION .)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${FNAME}.bin DESTINATION .)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE_INIT Release)

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

set(sources
    sbl-cpu.c
    start.S
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wno-nonnull -std=c99")
set(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -nostdlib -T ${PROJECT_SOURCE_DIR}/link.ld ${CMAKE_EXE_LINKER_FLAGS}")

add_executable(sbl-cpu.elf ${sources})

add_custom_command(TARGET sbl-cpu.elf PRE_BUILD COMMAND ${CMAKE_OBJCOPY} -I binary -O elf64-littleaarch64 -B aarch64
		   --rename-section .data=.rodata ${DDRINIT_PATH} ${CMAKE_CURRENT_BINARY_DIR}/ddrinit.o)
add_custom_command(TARGET sbl-cpu.elf PRE_BUILD COMMAND ${CMAKE_OBJCOPY} -I binary -O elf64-littleaarch64 -B aarch64
		   --rename-section .data=.rodata ${TFA_PATH} ${CMAKE_CURRENT_BINARY_DIR}/tfa.o)
add_custom_command(TARGET sbl-cpu.elf PRE_BUILD COMMAND ${CMAKE_OBJCOPY} -I binary -O elf64-littleaarch64 -B aarch64
		   --rename-section .data=.rodata ${UBOOT_PATH} ${CMAKE_CURRENT_BINARY_DIR}/u-boot.o)
add_custom_command(TARGET sbl-cpu.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY} -O binary sbl-cpu.elf sbl-cpu.bin)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sbl-cpu.bin DESTINATION .)

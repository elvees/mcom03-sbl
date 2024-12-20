# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.15)

set(CMAKE_BUILD_TYPE_INIT Release)

project(sbl-s3 ASM C)

set(WDT_RESET_INTERNAL FALSE CACHE BOOL "Allow internal WDT reset by sbl-s3 itself")

add_executable(${PROJECT_NAME}.elf
               exception-handler.c
               exception.S
               startup.S
               vectors.S
               main.c
               risc0-ipc/server/api.c
               risc0-ipc/server/ipc-ddr-subs.c
               risc0-ipc/server/ipc-init.c
               risc0-ipc/server/ipc-wdt.c)

if(WDT_ENABLE AND WDT_RESET_INTERNAL)
    target_compile_definitions(${PROJECT_NAME}.elf PRIVATE -DWDT_RESET_INTERNAL)
endif()

# Link static libraries
target_link_libraries(${PROJECT_NAME}.elf common)

target_link_options(${PROJECT_NAME}.elf PRIVATE
                    -T ${CMAKE_CURRENT_BINARY_DIR}/linkerscript.ld
                    -Wl,-Map=${PROJECT_NAME}.map)

add_custom_command(TARGET ${PROJECT_NAME}.elf PRE_LINK
                   COMMAND ${CMAKE_C_COMPILER}
                   $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS)
                   -E ${CMAKE_CURRENT_SOURCE_DIR}/link.ld.S -P
                   -o ${CMAKE_CURRENT_BINARY_DIR}/linkerscript.ld)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
                   COMMAND ${CMAKE_OBJDUMP}
                   -dx ${PROJECT_NAME}.elf > ${PROJECT_NAME}.dump
                   BYPRODUCTS ${PROJECT_NAME}.dump)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
                   COMMAND ${CMAKE_OBJCOPY}
                   -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
                   BYPRODUCTS ${PROJECT_NAME}.bin)

install(TARGETS ${PROJECT_NAME}.elf DESTINATION .)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.bin DESTINATION .)

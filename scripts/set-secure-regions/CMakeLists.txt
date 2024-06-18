# SPDX-License-Identifier: MIT

project(set-secure-regions ASM C)

add_executable(${PROJECT_NAME}.elf main.c startup.S)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(${PROJECT_NAME}.elf PRIVATE
                           -gdwarf-2 -fno-eliminate-unused-debug-types)
else()
    target_compile_options(${PROJECT_NAME}.elf PRIVATE -g0)
endif()

target_compile_options(${PROJECT_NAME}.elf PRIVATE
                       -Wall -Wno-nonnull -Werror -std=c99 -EL -mips32 -G0
                       -mabi=32 -fno-builtin-malloc -ffunction-sections
                       -fdata-sections)

target_link_options(${PROJECT_NAME}.elf PRIVATE
                    -T ${CMAKE_CURRENT_BINARY_DIR}/link.ld
                    -EL -nostdlib -mips32 -mabi=32 -G0 -Wl,--gc-sections
                    -Wl,-Map=${PROJECT_NAME}.map -Wl,--print-memory-usage)

# Link common static library
get_target_property(COMMON_INCLUDE_DIRECTORIES common INCLUDE_DIRECTORIES)
target_include_directories(${PROJECT_NAME}.elf PRIVATE ${COMMON_INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME}.elf common)

add_custom_command(TARGET ${PROJECT_NAME}.elf PRE_LINK COMMAND ${CMAKE_C_COMPILER}
                   $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS)
                   -E ${CMAKE_CURRENT_SOURCE_DIR}/link.ld.S -P
                   -o ${CMAKE_CURRENT_BINARY_DIR}/link.ld)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
                   COMMAND ${CMAKE_OBJDUMP}
                   -dx ${PROJECT_NAME}.elf > ${PROJECT_NAME}.dis
                   BYPRODUCTS ${PROJECT_NAME}.dis)

install(TARGETS ${PROJECT_NAME}.elf DESTINATION ./scripts)

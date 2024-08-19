# SPDX-License-Identifier: MIT

project(set-secure-regions ASM C)

add_executable(${PROJECT_NAME}.elf main.c startup.S)

target_link_options(${PROJECT_NAME}.elf PRIVATE
                    -T ${CMAKE_CURRENT_BINARY_DIR}/link.ld
                    -Wl,-Map=${PROJECT_NAME}.map)

# Link static libraries
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

# SPDX-License-Identifier: MIT

project(sbl-s2 ASM C)

add_executable(${PROJECT_NAME}.elf
               startup.S
               vectors.S
               main.c)

# Link static libraries
target_link_libraries(${PROJECT_NAME}.elf common)

target_compile_definitions(${PROJECT_NAME}.elf PRIVATE
                           -D_GNU_SOURCE)

target_link_options(${PROJECT_NAME}.elf PRIVATE
                    -T ${CMAKE_CURRENT_BINARY_DIR}/link.ld
                    -Wl,-Map=${PROJECT_NAME}.map -Wl,--cref)

generate_and_add_as_dependency(${PROJECT_NAME}.elf
                               ${CMAKE_CURRENT_SOURCE_DIR}/link.ld.S
                               ${CMAKE_CURRENT_BINARY_DIR}/link.ld)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
                   COMMAND ${CMAKE_OBJDUMP}
                   -dx ${PROJECT_NAME}.elf > ${PROJECT_NAME}.dump
                   BYPRODUCTS ${PROJECT_NAME}.dump)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
                   COMMAND ${CMAKE_OBJCOPY}
                   -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
                   BYPRODUCTS ${PROJECT_NAME}.bin)

install(TARGETS ${PROJECT_NAME}.elf DESTINATION ./sbl)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.bin DESTINATION ./sbl)

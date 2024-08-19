# SPDX-License-Identifier: MIT

project(SBL-XIP ASM C)

# The images directory must have the following structure:
# ./<ddrinit_N>/ddrinit.bin
# ./uboot-dtb/<dtb_name_N>.dtb
# ./u-boot.bin
# ./bl31.bin
# ./bl32.bin (optionally)
set(IMAGES_PATH CACHE STRING "Path to images directory")
if(NOT IMAGES_PATH)
    message(FATAL_ERROR "IMAGES_PATH is not specified")
endif()

if(EXISTS ${IMAGES_PATH}/bl32.bin)
    add_compile_options(-DHAS_BL32=1)
endif()

set(BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

file(READ ${DDRINIT_DTB_MAP_FILE} DDRINIT_DTB_MAP)
string(STRIP ${DDRINIT_DTB_MAP} DDRINIT_DTB_MAP)
string(REPLACE "\n" ";" DDRINIT_DTB_MAP ${DDRINIT_DTB_MAP})

foreach(DDRINIT_DTB_PAIR IN LISTS DDRINIT_DTB_MAP)
    # Make list from colon-divided pair to extract values
    string(REPLACE ":" ";" DDRINIT_DTB_LIST ${DDRINIT_DTB_PAIR})
    list(GET DDRINIT_DTB_LIST 0 DDRINIT_NAME)
    list(GET DDRINIT_DTB_LIST 1 DTB_NAME)

    # We need to put 3rd-party artifacts to different directories
    set(CMAKE_CURRENT_BINARY_DIR ${BINARY_DIR}/${DTB_NAME})
    set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    add_executable(sbl-xip-${DTB_NAME}.elf startup.S sbl.c)

    # Link static libraries
    target_link_libraries(sbl-xip-${DTB_NAME}.elf common)

    target_link_options(sbl-xip-${DTB_NAME}.elf PRIVATE -T ${CMAKE_CURRENT_BINARY_DIR}/link.ld)

    target_link_directories(sbl-xip-${DTB_NAME}.elf PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

    set_target_properties(sbl-xip-${DTB_NAME}.elf PROPERTIES
                          LINK_FLAGS "-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/sbl-xip.map")

    add_custom_command(TARGET sbl-xip-${DTB_NAME}.elf PRE_LINK COMMAND ${CMAKE_C_COMPILER}
                       $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS)
                       -E ${CMAKE_CURRENT_SOURCE_DIR}/link.ld.S -P
                       -o ${CMAKE_CURRENT_BINARY_DIR}/link.ld)

    add_custom_command(TARGET sbl-xip-${DTB_NAME}.elf PRE_BUILD COMMAND ${CMAKE_OBJCOPY} -I binary
                       -O elf32-littlemips -B mips --rename-section .data=.rodata
                       ${IMAGES_PATH}/ddrinit-${DDRINIT_NAME}/ddrinit.bin
                       ${CMAKE_CURRENT_BINARY_DIR}/ddrinit.o)

    add_custom_command(TARGET sbl-xip-${DTB_NAME}.elf PRE_BUILD COMMAND ${CMAKE_OBJCOPY} -I binary
                       -O elf32-littlemips -B mips --rename-section .data=.rodata
                       ${IMAGES_PATH}/uboot-dtb/${DTB_NAME}.dtb
                       ${CMAKE_CURRENT_BINARY_DIR}/dtb.o)

    add_custom_command(TARGET sbl-xip-${DTB_NAME}.elf PRE_BUILD COMMAND ${CMAKE_OBJCOPY} -I binary
                       -O elf32-littlemips -B mips --rename-section .data=.rodata
                       ${IMAGES_PATH}/bl31.bin ${CMAKE_CURRENT_BINARY_DIR}/tfa.o)

    if(EXISTS ${IMAGES_PATH}/bl32.bin)
        add_custom_command(TARGET sbl-xip-${DTB_NAME}.elf PRE_BUILD COMMAND ${CMAKE_OBJCOPY}
                           -I binary -O elf32-littlemips -B mips --rename-section .data=.rodata
                           ${IMAGES_PATH}/bl32.bin ${CMAKE_CURRENT_BINARY_DIR}/bl32.o)
    endif()

    add_custom_command(TARGET sbl-xip-${DTB_NAME}.elf PRE_BUILD COMMAND ${CMAKE_OBJCOPY} -I binary
                       -O elf32-littlemips -B mips --rename-section .data=.rodata
                       ${IMAGES_PATH}/u-boot.bin ${CMAKE_CURRENT_BINARY_DIR}/u-boot.o)

    add_custom_command(TARGET sbl-xip-${DTB_NAME}.elf POST_BUILD COMMAND ${CMAKE_OBJDUMP}
                       -dx sbl-xip-${DTB_NAME}.elf > ${CMAKE_CURRENT_BINARY_DIR}/sbl-xip.dis
                       BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/sbl-xip.dis)

    add_custom_command(TARGET sbl-xip-${DTB_NAME}.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
                       -O binary sbl-xip-${DTB_NAME}.elf sbl-xip-${DTB_NAME}.bin
                       BYPRODUCTS sbl-xip-${DTB_NAME}.bin)

    # Restore CMAKE_CURRENT_BINARY_DIR for successful artifacts installation
    set(CMAKE_CURRENT_BINARY_DIR ${BINARY_DIR})

    install(TARGETS sbl-xip-${DTB_NAME}.elf DESTINATION .)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sbl-xip-${DTB_NAME}.bin DESTINATION .)
endforeach()

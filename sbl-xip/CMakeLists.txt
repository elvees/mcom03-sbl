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
    string(REPLACE "," "-" DDRINIT_NAME ${DDRINIT_NAME})
    set(SBL_XIP_NAME "sbl-xip-${DDRINIT_NAME}-${DTB_NAME}")

    # We need to put 3rd-party artifacts to different directories
    set(CMAKE_CURRENT_BINARY_DIR ${BINARY_DIR}/${DDRINIT_NAME}-${DTB_NAME})
    set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    add_executable(${SBL_XIP_NAME}.elf startup.S sbl.c)

    # Link static libraries
    target_link_libraries(${SBL_XIP_NAME}.elf ${MipsCSP_LIBRARIES})

    target_link_options(${SBL_XIP_NAME}.elf PRIVATE -T ${CMAKE_CURRENT_BINARY_DIR}/link.ld)

    target_link_directories(${SBL_XIP_NAME}.elf PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

    set_target_properties(${SBL_XIP_NAME}.elf PROPERTIES
                          LINK_FLAGS "-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/sbl-xip.map")

    generate_and_add_as_dependency(${SBL_XIP_NAME}.elf
                                   ${CMAKE_CURRENT_SOURCE_DIR}/link.ld.S
                                   ${CMAKE_CURRENT_BINARY_DIR}/link.ld)

    generate_and_add_as_dependency(${SBL_XIP_NAME}.elf
                                   ${IMAGES_PATH}/ddrinit-${DDRINIT_NAME}/ddrinit.bin
                                   ${CMAKE_CURRENT_BINARY_DIR}/ddrinit.o)

    generate_and_add_as_dependency(${SBL_XIP_NAME}.elf
                                   ${IMAGES_PATH}/uboot-dtb/${DTB_NAME}.dtb
                                   ${CMAKE_CURRENT_BINARY_DIR}/dtb.o)

    generate_and_add_as_dependency(${SBL_XIP_NAME}.elf
                                   ${IMAGES_PATH}/bl31.bin
                                   ${CMAKE_CURRENT_BINARY_DIR}/tfa.o)

    if(EXISTS ${IMAGES_PATH}/bl32.bin)
        generate_and_add_as_dependency(${SBL_XIP_NAME}.elf
                                       ${IMAGES_PATH}/bl32.bin
                                       ${CMAKE_CURRENT_BINARY_DIR}/bl32.o)
    endif()

    generate_and_add_as_dependency(${SBL_XIP_NAME}.elf
                                   ${IMAGES_PATH}/u-boot.bin
                                   ${CMAKE_CURRENT_BINARY_DIR}/u-boot.o)

    add_custom_command(TARGET ${SBL_XIP_NAME}.elf POST_BUILD
                       COMMAND ${CMAKE_OBJDUMP}
                       -dx ${SBL_XIP_NAME}.elf > ${CMAKE_CURRENT_BINARY_DIR}/sbl-xip.dis
                       BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/sbl-xip.dis)

    add_custom_command(TARGET ${SBL_XIP_NAME}.elf POST_BUILD
                       COMMAND ${CMAKE_OBJCOPY}
                       -O binary ${SBL_XIP_NAME}.elf ${SBL_XIP_NAME}.bin
                       BYPRODUCTS ${SBL_XIP_NAME}.bin)

    # Restore CMAKE_CURRENT_BINARY_DIR for successful artifacts installation
    set(CMAKE_CURRENT_BINARY_DIR ${BINARY_DIR})

    install(TARGETS ${SBL_XIP_NAME}.elf DESTINATION ./sbl)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${SBL_XIP_NAME}.bin DESTINATION ./sbl)
endforeach()

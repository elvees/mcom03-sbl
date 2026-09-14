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

file(READ ${SBL_CONFIG_FILE} SBL_CONFIGS)
string(JSON DICT_LENGTH LENGTH "${SBL_CONFIGS}")
math(EXPR DICT_LENGTH "${DICT_LENGTH} - 1")

foreach(IDX RANGE ${DICT_LENGTH})
    string(JSON SBL_CONFIG GET ${SBL_CONFIGS} ${IDX})
    string(JSON BUILD_TYPE GET ${SBL_CONFIG} "boot_type")
    if(NOT "xip" STREQUAL BUILD_TYPE)
        continue()
    endif()

    string(JSON DTB_NAME GET ${SBL_CONFIG} "uboot_dtb")
    string(JSON DDRINIT_NAME GET ${SBL_CONFIG} "ddrinit_name")
    string(JSON SBL_XIP_NAME GET ${SBL_CONFIG} "sbl_xip_name")
    string(JSON DDRINIT_UBOOT_DTB GET ${SBL_CONFIG} "ddrinit_uboot_dtb")

    # We need to put 3rd-party artifacts to different directories
    set(CMAKE_CURRENT_BINARY_DIR ${BINARY_DIR}/${DDRINIT_UBOOT_DTB})
    set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    add_executable(${SBL_XIP_NAME}.elf startup.S sbl.c)

    if("${DTB_NAME}" MATCHES "^mcom03-")
        target_compile_definitions(drivers PUBLIC CHIP_MCOM03)
    else()
        target_compile_definitions(drivers PUBLIC CHIP_MCOM03R)
    endif()

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

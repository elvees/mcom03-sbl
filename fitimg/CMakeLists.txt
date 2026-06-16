# SPDX-License-Identifier: MIT

# The file is proceed if secure boot is enabled
if("${SBIMG_SEC_TYPE}" STREQUAL "none")
    return()
endif()

project(fitimg)

# Set vars
set(FIT_GEN_SCRIPT_IN ${CMAKE_CURRENT_SOURCE_DIR}/mcom03-fit-gen.template)
set(FIT_GEN_SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/mcom03-fit-gen)

configure_file(${FIT_GEN_SCRIPT_IN} ${FIT_GEN_SCRIPT} @ONLY)

# Read SBL build configuration file

file(READ ${SBL_CONFIG_FILE} SBL_CONFIGS)
string(JSON DICT_LENGTH LENGTH "${SBL_CONFIGS}")
math(EXPR DICT_LENGTH "${DICT_LENGTH} - 1")

# Iterate over SBL entries
foreach(IDX RANGE ${DICT_LENGTH})
    string(JSON SBL_CONFIG GET ${SBL_CONFIGS} ${IDX})
    string(JSON DTB_NAME GET ${SBL_CONFIG} "uboot_dtb")
    string(JSON DDRINIT_UBOOT_DTB GET ${SBL_CONFIG} "ddrinit_uboot_dtb")
    set(FITIMG_NAME "${DDRINIT_UBOOT_DTB}")

    # Set variables that depend on corresponding dtb values
    set(FIT_DTB_BIN ${IMAGES_PATH}/uboot-dtb/${DTB_NAME}.dtb)

    # Embed public key into U-Boot DTB
    if(NOT TARGET ${DTB_NAME}.keyed.dtb)
        add_custom_target(${DTB_NAME}.keyed.dtb
                          COMMAND fdt_add_pubkey -a ${FIT_SIGN_ALGO}
                                  -k ${FIT_KEYS} -n ${FIT_KEY_NAME}
                                  -r conf ${FIT_DTB_BIN}
                          DEPENDS ${FIT_DTB_BIN})

        # It is ok as the add_subdirectory command processes the CMakeLists.txt
        # file in the specified directory immediately upon being called. So the file
        # has to be processed after directory where {FITIMG_NAME}.tl-sbimg are created
        add_dependencies(${FITIMG_NAME}.tl-sbimg ${DTB_NAME}.keyed.dtb)
    endif()
endforeach()

# Install FIT gen script
install(PROGRAMS ${FIT_GEN_SCRIPT} DESTINATION .)

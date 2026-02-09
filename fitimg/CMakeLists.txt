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

# Read ddrinit-DTB map file
file(READ ${DDRINIT_DTB_MAP_FILE} DDRINIT_DTB_MAP)
string(STRIP ${DDRINIT_DTB_MAP} DDRINIT_DTB_MAP)
string(REPLACE "\n" ";" DDRINIT_DTB_MAP ${DDRINIT_DTB_MAP})

# Iterate over ddrinit:dtb pairs
foreach(DDRINIT_DTB_PAIR IN LISTS DDRINIT_DTB_MAP)
    # Make list from colon-divided pair to extract values
    string(REPLACE ":" ";" DDRINIT_DTB_LIST ${DDRINIT_DTB_PAIR})
    list(GET DDRINIT_DTB_LIST 1 DTB_NAME)

    # Set variables that depend on corresponding ddrinit:dtb pair
    set(FIT_DTB_BIN ${IMAGES_PATH}/uboot-dtb/${DTB_NAME}.dtb)

    # Embed public key into U-Boot DTB
    add_custom_target(${DTB_NAME}.keyed.dtb
                      COMMAND fdt_add_pubkey -a ${FIT_SIGN_ALGO}
                              -k ${FIT_KEYS} -n ${FIT_KEY_NAME}
                              -r conf ${FIT_DTB_BIN}
                      DEPENDS ${FIT_DTB_BIN})

    # It is ok as the add_subdirectory command processes the CMakeLists.txt
    # file in the specified directory immediately upon being called. So the file
    # has to be processed after directory where {DTB_NAME}.tl-sbimg are created
    add_dependencies(${DTB_NAME}.tl-sbimg ${DTB_NAME}.keyed.dtb)
endforeach()

# Install FIT gen script
install(PROGRAMS ${FIT_GEN_SCRIPT} DESTINATION .)

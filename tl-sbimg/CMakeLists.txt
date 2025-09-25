# SPDX-License-Identifier: MIT

project(tl-sbimg)

# Set vars
set(SBIMG_MKSBIMG ${SBIMG_BOOTROM_TOOLS_INSTALL_DIR}/mksbimage/mksbimage)
set(SBIMG_OTPCREATOR ${SBIMG_BOOTROM_TOOLS_INSTALL_DIR}/otpcreator/otpcreator)
set(SBIMG_BOOTROM_UTILS_CFG_IN ${CMAKE_CURRENT_SOURCE_DIR}/bootrom-none.cfg.in)
set(SBIMG_SBL_UTILS_CFG_IN ${CMAKE_CURRENT_SOURCE_DIR}/sbl-${SBIMG_SEC_TYPE}.cfg.in)
set(SBIMG_BINARIES_DIR ${CMAKE_CURRENT_BINARY_DIR}/build)

if("${SBIMG_MAKE_RECOVERY}" STREQUAL "y")
    set(SBIMG_PKG_TOML_IN ${CMAKE_CURRENT_SOURCE_DIR}/package.template.recovery.toml)
else()
    set(SBIMG_PKG_TOML_IN ${CMAKE_CURRENT_SOURCE_DIR}/package.template.toml)
endif()

get_target_property(SBIMG_SBL_S1_BIN_DIR sbl-s1.elf BINARY_DIR)
set(SBIMG_SBL_S1_BIN ${SBIMG_SBL_S1_BIN_DIR}/sbl-s1.bin)

get_target_property(SBIMG_SBL_S2_BIN_DIR sbl-s2.elf BINARY_DIR)
set(SBIMG_SBL_S2_BIN ${SBIMG_SBL_S2_BIN_DIR}/sbl-s2.bin)

get_target_property(SBIMG_SBL_S3_BIN_DIR sbl-s3.elf BINARY_DIR)
set(SBIMG_SBL_S3_BIN ${SBIMG_SBL_S3_BIN_DIR}/sbl-s3.bin)

set(SBIMG_BL31_BIN ${IMAGES_PATH}/bl31.bin)
set(SBIMG_UBOOT_BIN ${IMAGES_PATH}/u-boot.bin)

set(SBIMG_OTP_IMG ${SBIMG_BINARIES_DIR}/sbl-otp.bin)
get_filename_component(OTP_IMG "${SBIMG_OTP_IMG}" NAME)

# Generate dir where to store artifacts
file(MAKE_DIRECTORY ${SBIMG_BINARIES_DIR})

# Parse DDRINIT_DTB_MAP_FILE
file(READ ${DDRINIT_DTB_MAP_FILE} DDRINIT_DTB_MAP)
string(STRIP ${DDRINIT_DTB_MAP} DDRINIT_DTB_MAP)
string(REPLACE "\n" ";" DDRINIT_DTB_MAP ${DDRINIT_DTB_MAP})

# Generate OTP
if("${SBIMG_SEC_TYPE}" STREQUAL "none")
    add_custom_command(OUTPUT ${SBIMG_OTP_IMG}
                       COMMAND ${SBIMG_OTPCREATOR} -o ${SBIMG_OTP_IMG})
elseif("${SBIMG_SEC_TYPE}" STREQUAL "sign")
    add_custom_command(OUTPUT ${SBIMG_OTP_IMG}
                       COMMAND ${SBIMG_OTPCREATOR} -o ${SBIMG_OTP_IMG}
                               -S -R ${SBIMG_ROOT_CA}
                       DEPENDS ${SBIMG_ROOT_CA})
elseif("${SBIMG_SEC_TYPE}" STREQUAL "enc")
    add_custom_command(OUTPUT ${SBIMG_OTP_IMG}
                       COMMAND ${SBIMG_OTPCREATOR} -o ${SBIMG_OTP_IMG}
                               -S -R ${SBIMG_ROOT_CA}
                               -E -N ${SBIMG_ENC_DSN}
                               -K ${SBIMG_ENC_DUK}
                       DEPENDS ${SBIMG_ROOT_CA})
else()
    message(FATAL_ERROR "Unsupported type tl-sbimg: \"${SBIMG_SEC_TYPE}\"")
endif()

# Iterate over ddrinit:dtb pairs
foreach(DDRINIT_DTB_PAIR IN LISTS DDRINIT_DTB_MAP)
    # Make list from colon-divided pair to extract values
    string(REPLACE ":" ";" DDRINIT_DTB_LIST ${DDRINIT_DTB_PAIR})
    list(GET DDRINIT_DTB_LIST 0 DDRINIT_NAME)
    list(GET DDRINIT_DTB_LIST 1 DTB_NAME)

    # Store artifacts in corresponding directories
    set(CMAKE_CURRENT_BINARY_DIR ${SBIMG_BINARIES_DIR}/${DTB_NAME})
    set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    # Set variables that depend on corresponding ddrinit:dtb pair
    set(SBIMG_DDRINIT_BIN ${IMAGES_PATH}/ddrinit-${DDRINIT_NAME}-bootrom/ddrinit.bin)
    set(SBIMG_DTB_BIN ${IMAGES_PATH}/uboot-dtb/${DTB_NAME}.dtb)

    set(SBIMG_BOOTROM_UTILS_CFG
        ${CMAKE_CURRENT_BINARY_DIR}/bootrom-none-${DDRINIT_NAME}.cfg)
    set(SBIMG_SBL_UTILS_CFG ${CMAKE_CURRENT_BINARY_DIR}/sbl-${SBIMG_SEC_TYPE}-${DTB_NAME}.cfg)

    set(SBIMG_BOOTROM_IMG
        ${CMAKE_CURRENT_BINARY_DIR}/bootrom-none-${DDRINIT_NAME}.sbimg)
    set(SBIMG_PKG_TOML ${CMAKE_CURRENT_BINARY_DIR}/package.toml)

    if("${SBIMG_MAKE_RECOVERY}" STREQUAL "y")
        set(SBIMG_SBL_IMG
            ${CMAKE_CURRENT_BINARY_DIR}/sbl-${SBIMG_SEC_TYPE}-${DTB_NAME}-recovery.sbimg)
        set(SBIMG_TAR ${CMAKE_CURRENT_BINARY_DIR}/${DTB_NAME}-recovery.tl-image)
    else()
        set(SBIMG_SBL_IMG ${CMAKE_CURRENT_BINARY_DIR}/sbl-${SBIMG_SEC_TYPE}-${DTB_NAME}.sbimg)
        set(SBIMG_TAR ${CMAKE_CURRENT_BINARY_DIR}/${DTB_NAME}.tl-image)
    endif()
    get_filename_component(PKG_TOML "${SBIMG_PKG_TOML}" NAME)
    get_filename_component(BOOTROM_SBIMG "${SBIMG_BOOTROM_IMG}" NAME)
    get_filename_component(SBL_SBIMG "${SBIMG_SBL_IMG}" NAME)

    # Set the files that are used to build the tl-sbimg image depending on the secure image type
    set(SBIMG_TO_TAR -C ${CMAKE_CURRENT_BINARY_DIR} ${PKG_TOML}
                     -C ${CMAKE_CURRENT_BINARY_DIR} ${BOOTROM_SBIMG}
                     -C ${CMAKE_CURRENT_BINARY_DIR} ${SBL_SBIMG}
                     -C ${SBIMG_BINARIES_DIR} ${OTP_IMG})
    set(SBIMG_TAR_DEPENDS ${SBIMG_PKG_TOML}
                          ${SBIMG_BOOTROM_IMG}
                          ${SBIMG_SBL_IMG}
                          ${SBIMG_OTP_IMG})

    # Configure files before building process
    configure_file(${SBIMG_BOOTROM_UTILS_CFG_IN}
                   ${SBIMG_BOOTROM_UTILS_CFG} @ONLY)

    configure_file(${SBIMG_SBL_UTILS_CFG_IN}
                   ${SBIMG_SBL_UTILS_CFG} @ONLY)

    configure_file(${SBIMG_PKG_TOML_IN}
                   ${SBIMG_PKG_TOML} @ONLY)

    # Generate bootrom sbimg image
    add_custom_command(OUTPUT ${SBIMG_BOOTROM_IMG}
                       COMMAND ${SBIMG_MKSBIMG}
                               -i ${SBIMG_BOOTROM_UTILS_CFG}
                               -o ${SBIMG_BOOTROM_IMG}
                       DEPENDS ${SBIMG_DDRINIT_BIN}
                               ${SBIMG_SBL_S1_BIN}
                               ${SBIMG_SBL_S2_BIN})

    # Generate sbl sbimg image
    add_custom_command(OUTPUT ${SBIMG_SBL_IMG}
                       COMMAND ${SBIMG_MKSBIMG}
                               -i ${SBIMG_SBL_UTILS_CFG}
                               -o ${SBIMG_SBL_IMG}
                       DEPENDS ${SBIMG_BL31_BIN}
                               ${SBIMG_UBOOT_BIN}
                               ${SBIMG_DTB_BIN}
                               ${SBIMG_SBL_S3_BIN})

    # Generate tl-sbimg image
    add_custom_command(OUTPUT ${SBIMG_TAR}
                       COMMAND tar -czf ${SBIMG_TAR} ${SBIMG_TO_TAR}
                       DEPENDS ${SBIMG_TAR_DEPENDS})

    add_custom_target(${DTB_NAME}.tl-sbimg ALL DEPENDS ${SBIMG_TAR})

    add_dependencies(${DTB_NAME}.tl-sbimg sbl-s1.elf
                                          sbl-s2.elf
                                          sbl-s3.elf)

    # Install tl-sbimg images
    install(FILES ${SBIMG_TAR} DESTINATION ./tl-sbimg)

    # Restore CMAKE_CURRENT_BINARY_DIR for successful artifacts installation
    set(CMAKE_CURRENT_BINARY_DIR ${SBIMG_BINARIES_DIR})
endforeach()

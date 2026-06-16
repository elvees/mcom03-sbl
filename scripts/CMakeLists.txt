# SPDX-License-Identifier: MIT

project(scripts ASM C)

add_subdirectory(set-secure-regions)

install(FILES
        helpers.py
        mcom03-boot.mdb
        mcom03-boot-s3.mdb
        ${SBL_CONFIG_FILE}
        DESTINATION ./sbl/scripts)

# SPDX-License-Identifier: MIT

project(scripts ASM C)

add_subdirectory(set-secure-regions)

install(FILES
        helpers.py
        mcom03-boot.mdb
        mcom03-boot-tl.mdb
        ${DDRINIT_DTB_MAP_FILE}
        DESTINATION ./scripts)

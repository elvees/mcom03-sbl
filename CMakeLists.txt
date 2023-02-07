# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.15)

set(CMAKE_BUILD_TYPE_INIT Release)

project(sbl)

add_subdirectory(sbl-s1)
add_subdirectory(sbl-xip)

install(FILES
        scripts/mcom03-boot.mdb
        scripts/mcom03-boot-tl.mdb
        DESTINATION ./scripts)

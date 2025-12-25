# SPDX-License-Identifier: MIT

# MipsCSP_LIBRARIES
#    Provides list of MipsCSP libraries.

if(NOT DEFINED MipsCSP_INCLUDE_DIRS OR NOT DEFINED MipsCSP_LIBRARIES)
    set(BUILD_TARGET COMMON_LIBS CACHE STRING "Build SBL common library" FORCE)

    set(MipsCSP_SOURCE "${CMAKE_CURRENT_LIST_DIR}/..")

    include(FetchContent)
    FetchContent_Declare(MipsCSP
        SOURCE_DIR ${MipsCSP_SOURCE}
    )
    FetchContent_MakeAvailable(MipsCSP)

    set(MipsCSP_LIBRARIES mips-csp)

    include_directories(${MipsCSP_SOURCE})

    unset(BUILD_TARGET CACHE)
    unset(MipsCSP_SOURCE)
endif()

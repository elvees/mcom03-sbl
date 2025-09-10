# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.15)

project(sbl-unittests C CXX)

include(GNUInstallDirs)

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME}.elf
    unittest-env.cc
    ${CMAKE_SOURCE_DIR}/libs/env/env.c
    ${CMAKE_SOURCE_DIR}/libs/env/env-io.c
    ${CMAKE_SOURCE_DIR}/libs/env/env-io-ram.c
    ${CMAKE_SOURCE_DIR}/libs/env/env-io-spi.c
    ${CMAKE_SOURCE_DIR}/third-party/crc/crc32.c
)

target_link_libraries(${PROJECT_NAME}.elf PRIVATE
    ${GTEST_BOTH_LIBRARIES}
    ${THREADS_LIBRARIES}
)

target_include_directories(${PROJECT_NAME}.elf PRIVATE
    ${GTEST_INCLUDE_DIRS}
)

target_compile_definitions(${PROJECT_NAME}.elf PRIVATE
    -DLOG_LEVEL=40
    -DENABLE_ENV_SPI=0
    -DENABLE_ENV_RAM=1
)

target_compile_options(${PROJECT_NAME}.elf PRIVATE
    -O0
    -g
    -m64
    --coverage
    -fno-omit-frame-pointer
    -fno-sanitize-recover=all
    -fsanitize=address,undefined
)

target_link_options(${PROJECT_NAME}.elf PRIVATE
    -m64
    --coverage
    -fsanitize=address,undefined
    -lc
    -lgcov
    -pthread
    -Wl,--cref
    -Wl,-Map=${PROJECT_NAME}.map
)

#!/usr/bin/env bash
# Copyright 2026 RnD Center "ELVEES", JSC
#
# This script builds default SBL images of all security.
# Using default parameters, the script can be used to generate and test firmware images.

set -exuo pipefail

readonly REPO_TOPDIR=$(realpath $(dirname $0)/..)

fit_certs=${fit_certs:?Certificates were not specified}
sbimg_certs=${sbimg_certs:?Certificates were not specified}
images_path=${images_path:?The path to the images is not specified}
ddrinit_dtb_map=${ddrinit_dtb_map:?DTB map file is not specified}
cmake_toolchain_file=${cmake_toolchain_file:?CMake toolchain file is not specified}

certs_dir=${certs_dir:-$REPO_TOPDIR/certs}
build_dir=${build_dir:-$REPO_TOPDIR/build}
install_dir=${install_dir:-$images_path}
build_id=${build_id:-""}

readonly sec_type=( none sign enc )
readonly targets_subdirs=(
    "sbl-s1 sbl-s2 sbl-s3 sbl-xip scripts tl-sbimg"
    "fitimg tl-sbimg"
    "tl-sbimg"
)
readonly extra_build_args=(
    ""

    "-DSBIMG_ROOT_CA=$certs_dir/sbimg/rootCA.der \
    -DSBIMG_NON_ROOT_CA=$certs_dir/sbimg/nonRootCA.der \
    -DSBIMG_FW_CA=$certs_dir/sbimg/fwCA.der \
    -DSBIMG_FW_PK=$certs_dir/sbimg/fwPrivateKey.der \
    -DFIT_KEYS=$certs_dir/fit \
    -DFIT_KEY_NAME=root \
    -DFIT_SIGN_ALGO=sha256,rsa3072"

    "-DSBIMG_ENC_DSN=0x00010203
    -DSBIMG_ENC_DUK=0x000102030405060708090a0b0c0d0e0f \
    -DSBIMG_ENC_KEY=0x000102030405060708090a0b0c0d0e0f \
    -DSBIMG_ROOT_CA=$certs_dir/sbimg/rootCA.der \
    -DSBIMG_NON_ROOT_CA=$certs_dir/sbimg/nonRootCA.der \
    -DSBIMG_FW_CA=$certs_dir/sbimg/fwCA.der \
    -DSBIMG_FW_PK=$certs_dir/sbimg/fwPrivateKey.der \
    -DFIT_KEYS=$certs_dir/fit \
    -DFIT_KEY_NAME=root \
    -DFIT_SIGN_ALGO=sha256,rsa3072"
)

check_files="$fit_certs \
    $sbimg_certs \
    $images_path \
    $ddrinit_dtb_map \
    $cmake_toolchain_file \
    $certs_dir \
    $build_dir \
    $install_dir"

for file in $check_files; do
    if [[ "$file" != /* ]]; then
        echo "Path '$file' is not absolute" >&2
        exit 1
    fi
done

mkdir -p $certs_dir/sbimg
mkdir -p $certs_dir/fit
tar xf $sbimg_certs -C $certs_dir/sbimg
tar xf $fit_certs -C $certs_dir/fit

for index in "${!sec_type[@]}"; do
    CFLAGS='-Wall -Werror' cmake -S $REPO_TOPDIR -B $build_dir \
        -DIMAGES_PATH=$images_path \
        -DDDRINIT_DTB_MAP_FILE=$ddrinit_dtb_map \
        -DCMAKE_TOOLCHAIN_FILE=$cmake_toolchain_file \
        -DSBIMG_SEC_TYPE=${sec_type[$index]} \
        -DSBIMG_MAKE_RECOVERY= \
        -DBUILD_ID=$build_id \
        -DCMAKE_INSTALL_PREFIX= \
        ${extra_build_args[$index]}

    for subdir in ${targets_subdirs[$index]}; do
        make -C $build_dir/$subdir -j $(nproc)
        make DESTDIR=$install_dir install/fast -C $build_dir/$subdir
    done

    mv $install_dir/tl-sbimg $install_dir/tl-sbimg-${sec_type[$index]}
done

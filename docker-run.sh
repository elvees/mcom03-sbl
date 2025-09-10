#!/usr/bin/env bash
# Copyright 2025 RnD Center "ELVEES", JSC

set -exuo pipefail

readonly THIS_FILE_DIR=$(dirname $(readlink -f $0))
readonly DOCKER_IMAGE_TAG=elvees:mcom03-sbl-host-env
DOCKERFILE=${DOCKERFILE:-$THIS_FILE_DIR/Dockerfile}
interactive=$(tty --silent && echo --interactive --tty || echo)

docker build - < $DOCKERFILE --tag $DOCKER_IMAGE_TAG

docker run --rm \
    \
    -v $THIS_FILE_DIR:$THIS_FILE_DIR \
    \
    --workdir=$THIS_FILE_DIR \
    -u $(id -u):$(id -g) \
    $interactive \
    $DOCKER_IMAGE_TAG \
    /bin/bash -cuxe "$*"

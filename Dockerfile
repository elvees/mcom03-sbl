FROM docker.io/library/rockylinux:9

RUN dnf update -y && \
    dnf install -y dnf-plugins-core epel-release && \
    dnf install -y \
        cmake \
        gcc \
        gdb \
        g++ \
        gtest-devel \
        make \
        lcov \
        libasan \
        libubsan

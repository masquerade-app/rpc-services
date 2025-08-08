# syntax=docker/dockerfile:1

# Dockerfile for dev environment

FROM ubuntu:noble AS base
#RUN useradd -m -s /bin/bash msqrd-dev

# Download gRPC build dependencies
RUN apt-get update -y \
    && apt-get install -y \
    git \
    build-essential \
    autoconf \
    libtool \
    pkg-config \
    cmake \
    && apt-get upgrade -y \
    && apt-get clean

# Build and install gRPC
WORKDIR /deps
RUN git clone --recurse-submodules -b v1.73.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc
WORKDIR /deps/grpc/cmake/build
RUN cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_CXX_STANDARD=17 \
      -DCMAKE_INSTALL_PREFIX=/deps \
      ../.. \
      && make -j 8 \
      && make install

# Download project dependencies
RUN apt-get install -y \
    clang \
    clang-format \
    clangd \
    clang-tidy \
    sqlite3 \
    python3-pip \
    pipx \
    cppcheck \
    && apt-get upgrade -y \
    && apt-get clean \
    && pipx install cpplint \
    && pipx ensurepath
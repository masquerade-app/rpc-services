# syntax=docker/dockerfile:1

# Dockerfile for dev environment

FROM ubuntu:noble AS base
#RUN useradd -m -s /bin/bash msqrd-dev

# Download tool dependencies
RUN apt-get update -y \
    && apt-get install -y \
    git \
    clang \
    clang-format \
    clangd \
    clang-tidy \
    build-essential \
    autoconf \
    libtool \
    pkg-config \
    sudo \
    sqlite3 \
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
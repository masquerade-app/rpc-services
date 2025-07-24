# syntax=docker/dockerfile:1

# Dockerfile for release binary

FROM ubuntu:latest AS base

FROM base AS build
ENV INSTALL_DIR="/home/masquerade/deps"

# Download dependencies
RUN apt-get update -y \
    && apt-get upgrade -y \
    && apt-get install -y \
    git clang clang-format \
    build-essential autoconf \
    libtool pkg-config cmake \
    && export PATH="${INSTALL_DIR}/bin:$PATH" \
    && apt-get clean

# Build and install gRPC
WORKDIR ${INSTALL_DIR}
RUN git clone --recurse-submodules -b v1.73.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc
WORKDIR ${INSTALL_DIR}/grpc/cmake/build
RUN cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_CXX_STANDARD=17 \
      -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
      ../.. \
      && make -j 4 \
      && make install

COPY . /home/masquerade

# build source code in release mode

# FROM base AS release
# copy from build binary to local dir
# syntax=docker/dockerfile:1

# Dockerfile for release binary

FROM ubuntu:latest AS base

FROM base AS build

# build source code in release mode

# FROM base AS release
# copy from build binary to local dir
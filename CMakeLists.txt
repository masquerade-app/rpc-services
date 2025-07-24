cmake_minimum_required(VERSION 3.22...4.0)
project(
  masquerade
  VERSION 0.1
  DESCRIPTION "Server"
  LANGUAGES CXX
)

# Variables
set(CMAKE_CXX_STANDARD 23)                # Compile with C++23
set(CMAKE_CXX_STANDARD_REQUIRED True)     # C++23 required
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)     # Creates compile_commands.json in build folder
set(CMAKE_PREFIX_PATH /deps/lib/cmake)    # Tell packages where to look for dependencies
set(CMAKE_BUILD_TYPE Debug)               # Build mode: Debug | Release
#set(CMAKE_CXX_CLANG_TIDY clang-tidy)     # Enable clang-tidy with .clang-tidy file
add_compile_options(-Wall -Wextra)        # Compile flags

find_package(absl CONFIG REQUIRED)
find_package(utf8_range CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# Bring subdirectories into scope
add_subdirectory(src/services)
#add_subdirectory(src/test)
add_subdirectory(src/genproto)

# Executable
add_executable(
  masquerade
  src/main.cc
)

# Include paths
target_include_directories(
  masquerade
  PRIVATE
  src/services
  /deps/include
)

# Dependencies
target_link_libraries(
  masquerade
  PRIVATE
  services
  gRPC::grpc++
  gRPC::grpc++_reflection
)
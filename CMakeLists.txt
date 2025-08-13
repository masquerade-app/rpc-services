cmake_minimum_required(VERSION 3.28...4.0)

set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++ -Wall -Wextra -Wno-builtin-macro-redefined -D__cpp_concepts=202002L")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_PREFIX_PATH /deps/lib/cmake)
if(STATIC_CHECK=ON)
  set(CMAKE_CXX_CLANG_TIDY clang-tidy)
endif()

project(
  masquerade
  VERSION 0.1
  DESCRIPTION "Server"
  LANGUAGES CXX C
)

find_package(absl CONFIG REQUIRED)
find_package(utf8_range CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

include_directories(.)
include_directories(/deps/include)

# Bring subdirectories into scope
add_subdirectory(src/util)
add_subdirectory(src/services)
add_subdirectory(src/genproto)
add_subdirectory(src/database)

# Executable
add_executable(
  masquerade
  src/main.cc
)

# Dependencies
target_link_libraries(
  masquerade
  PRIVATE
  services
  database
  gRPC::grpc++
  gRPC::grpc++_reflection
)

# Tests
if(BUILD_TESTING)
  include(CTest)
  add_subdirectory(test)
endif()
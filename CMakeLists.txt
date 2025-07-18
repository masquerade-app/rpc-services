cmake_minimum_required(VERSION 2.8...4.0)
project(
  masquerade
  VERSION 0.1
  DESCRIPTION "Server"
  LANGUAGES CXX
)

# Variables
set(CMAKE_CXX_STANDARD 20)                # Compile with C++20
set(CMAKE_CXX_STANDARD_REQUIRED True)     # C++20 required
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)     # Creates compile_commands.json in build folder
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)     # External packages must use at least CMake v3.5
set(CMAKE_BUILD_PARALLEL_LEVEL 8)         # Use 8 threads when building
set(CMAKE_PREFIX_PATH build/_deps)        # Tell packages where to look for dependencies
set(BUILD_MODE Debug)                     # Build mode: Debug | Release
set(CMAKE_BUILD_TYPE ${BUILD_MODE})
#set(CMAKE_CXX_CLANG_TIDY clang-tidy)     # Enable clang-tidy with .clang-tidy file
add_compile_options(-Wall -Wextra)        # Compile flags

# Fetch dependencies
include(FetchContent)

# googletest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        52eb8108c5bdec04579160ae17225d66034bd723  # v1.17.0
)
FetchContent_MakeAvailable(googletest)

# grpc
set(ABSL_ENABLE_INSTALL ON)               # Install absl library
set(ABSL_BUILD_TESTS OFF)                 # Do not build absl tests
set(protobuf_BUILD_TESTS OFF)             # Do not build protobuf tests
set(protobuf_LOCAL_DEPENDENCIES_ONLY ON)  # Build protobuf from local source
set(gRPC_BUILD_TESTS OFF)                 # Do not build grpc tests
FetchContent_Declare(
  gRPC
  GIT_REPOSITORY https://github.com/grpc/grpc.git
  GIT_TAG        6eae42baf0dc7950a8c25d227575a0d24c9aa286  # v1.73.1
)
FetchContent_MakeAvailable(gRPC)

# Bring subdirectories into scope
add_subdirectory(src/services)
add_subdirectory(src/test)
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
  build/_deps/grpc-src/third_party/abseil-cpp
  build/_deps/grpc-src/include
)

# Dependencies
# Uncomment absl deps as needed
target_link_libraries(
  masquerade
  PRIVATE
  services
  grpc++
  grpc++_reflection
  #absl::algorithm
  #absl::base
  #absl::debugging
  #absl::flat_hash_map
  absl::flags
  #absl::memory
  #absl::meta
  #absl::numeric
  #absl::random_random
  absl::strings
  #absl::synchronization
  #absl::time
  #absl::utility
)
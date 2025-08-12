set(CMAKE_CXX_CLANG_TIDY "")

file(GLOB SOURCES "./*.cc")
file(GLOB HEADERS "./*.h")

add_library(
  genproto
  STATIC
  ${SOURCES}
)

target_include_directories(
  genproto
  PUBLIC
  /deps/include
  /deps/grpc/third_party/protobuf/src
)

target_link_libraries(
  genproto
  PUBLIC
  gRPC::grpc++
)

# Generated code
set_source_files_properties(
  ${SOURCES}
  ${HEADERS}
  PROPERTIES
  SKIP_LINTING ON
)
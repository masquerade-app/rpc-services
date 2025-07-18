add_library(
  genproto
  STATIC
  helloworld.pb.h
  helloworld.pb.cc
  helloworld.grpc.pb.h
  helloworld.grpc.pb.cc
)

target_include_directories(
  genproto
  PUBLIC
  build/_deps/grpc-src/include
  build/_deps/grpc-src/third_party/protobuf/src
)

target_link_libraries(
  genproto
  PUBLIC
  protobuf
  grpc++
)

# Generated code
set_source_files_properties(
  helloworld.pb.h
  helloworld.pb.cc
  helloworld.grpc.pb.h
  helloworld.grpc.pb.cc
  PROPERTIES
  SKIP_LINTING ON
)
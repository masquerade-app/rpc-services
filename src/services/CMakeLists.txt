add_library(
  services
  STATIC
  admin_service.cc
  helloworld_service.cc
)

target_include_directories(
  services
  PUBLIC
  ..
  /deps/include
)

target_link_libraries(
  services
  PUBLIC
  genproto
  gRPC::grpc++
)

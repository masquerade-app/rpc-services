add_library(
  services
  STATIC
  admin_service.cc
)

target_link_libraries(
  services
  PUBLIC
  genproto
  gRPC::grpc++
)
add_library(
  services
  STATIC
  admin_service.cc
  admin_service.h
  profile_service.cc
  profile_service.h
)

target_link_libraries(
  services
  PUBLIC
  genproto
  gRPC::grpc++
)
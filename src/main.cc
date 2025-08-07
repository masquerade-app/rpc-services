#include <cstdlib>
#include <string>

// #include <grpcpp/ext/proto_server_reflection_plugin.h>
// #include <grpcpp/grpcpp.h>
// #include "services/helloworld_service.h"

int main() {
  std::string server_address = "0.0.0.0:50051";

  /*
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&greeter_service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  server->Wait();
  */
  return EXIT_SUCCESS;
}

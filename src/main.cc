#include <iostream>
#include <memory>
#include <string>

#include <absl/strings/str_cat.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include "services/admin_service.h"
#include "services/helloworld_service.h"


int main() {
  std::cout << absl::StrCat("Haha! ", admin::helloWorld());

  std::string server_address = "0.0.0.0:50051";

  GreeterImpl greeter_service;

  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&greeter_service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << "\n";

  server->Wait();
  return 0;
}

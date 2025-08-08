// Copyright © 2025 William Huffman

#include "services/helloworld_service.h"

#include <string>

using grpc::Status, grpc::ServerContext, helloworld::HelloWorldRequest,
    helloworld::HelloWorldResponse;

Status GreeterImpl::HelloWorld(ServerContext* context,
                               const HelloWorldRequest* request,
                               HelloWorldResponse* response) {
  if (context == nullptr) {
    return Status::CANCELLED;
  }
  const std::string& message = request->message();
  int64_t increment = request->increment();

  if (message == "increment") {
    increment += 1;
    response->set_message("incremented value");
  } else if (message == "decrement") {
    increment -= 1;
    response->set_message("decremented value");
  } else {
    response->set_message("unrecognized command");
  }

  response->set_current_count(increment);
  return Status::OK;
}

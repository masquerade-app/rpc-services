#ifndef HELLOWORLD_SERVICE_H
#define HELLOWORLD_SERVICE_H


#include <grpcpp/grpcpp.h>
#include "helloworld.grpc.pb.h"

class GreeterImpl final : public helloworld::Greeter::Service {
  grpc::Status HelloWorld(grpc::ServerContext* context,  // NOLINT
                          const helloworld::HelloWorldRequest* request,
                          helloworld::HelloWorldResponse* response) override {
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
    return grpc::Status::OK;
  }
};


#endif

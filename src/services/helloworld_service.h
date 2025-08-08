// Copyright © 2025 William Huffman

#ifndef SRC_SERVICES_HELLOWORLD_SERVICE_H_
#define SRC_SERVICES_HELLOWORLD_SERVICE_H_

#include <grpcpp/grpcpp.h>

#include "genproto/helloworld.grpc.pb.h"

class GreeterImpl final : public helloworld::Greeter::Service {
  grpc::Status HelloWorld(grpc::ServerContext* context,
                          const helloworld::HelloWorldRequest* request,
                          helloworld::HelloWorldResponse* response) override;
};

#endif  // SRC_SERVICES_HELLOWORLD_SERVICE_H_

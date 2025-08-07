#ifndef MASQUERADE_RPC_SERVICES_SERVICES_HELLOWORLD_SERVICE_H
#define MASQUERADE_RPC_SERVICES_SERVICES_HELLOWORLD_SERVICE_H

#include <grpcpp/grpcpp.h>

#include "genproto/helloworld.grpc.pb.h"

class GreeterImpl final : public helloworld::Greeter::Service {
  grpc::Status HelloWorld(grpc::ServerContext* context,  // NOLINT
                          const helloworld::HelloWorldRequest* request,
                          helloworld::HelloWorldResponse* response) override;
};

#endif

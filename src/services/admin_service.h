// Copyright © 2025 William Huffman

#ifndef SRC_SERVICES_ADMIN_SERVICE_H_
#define SRC_SERVICES_ADMIN_SERVICE_H_

#include <grpcpp/grpcpp.h>

#include "src/genproto/admin.grpc.pb.h"
#include "src/genproto/admin.pb.h"

namespace masquerade {

struct AdminService final : public admin::Admin::CallbackService {
  grpc::ServerUnaryReactor* CreateAccount(grpc::CallbackServerContext* context,
                                          const admin::Account* request,
                                          admin::AdminResponse* response) override;

  grpc::ServerUnaryReactor* GetAccount(grpc::CallbackServerContext* context,
                                       const admin::AccountRequest* request,
                                       admin::Account* response) override;

  grpc::ServerUnaryReactor* UpdateAccount(grpc::CallbackServerContext* context,
                                          const admin::Account* request,
                                          admin::AdminResponse* response) override;

  grpc::ServerUnaryReactor* DeleteAccount(grpc::CallbackServerContext* context,
                                          const admin::AccountRequest* request,
                                          admin::AdminResponse* response) override;
};

}  // namespace masquerade

#endif  // SRC_SERVICES_ADMIN_SERVICE_H_

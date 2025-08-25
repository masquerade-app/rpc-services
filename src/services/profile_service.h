// Copyright © 2025 William Huffman

#ifndef SRC_SERVICES_PROFILE_SERVICE_H_
#define SRC_SERVICES_PROFILE_SERVICE_H_

#include <grpcpp/grpcpp.h>

#include "src/genproto/profile.grpc.pb.h"
#include "src/genproto/profile.pb.h"

namespace masquerade {

struct ProfileService final : public profile::Profile::CallbackService {
  grpc::ServerUnaryReactor* CreateProfile(grpc::CallbackServerContext* context,
                                          const profile::ProfileCard* request,
                                          profile::ProfileResponse* response) override;

  grpc::ServerUnaryReactor* GetProfile(grpc::CallbackServerContext* context,
                                       const profile::ProfileRequest* request,
                                       profile::ProfileCard* response) override;

  grpc::ServerUnaryReactor* UpdateProfile(grpc::CallbackServerContext* context,
                                          const profile::ProfileCard* request,
                                          profile::ProfileResponse* response) override;

  grpc::ServerUnaryReactor* ClearProfile(grpc::CallbackServerContext* context,
                                         const profile::ProfileRequest* request,
                                         profile::ProfileResponse* response) override;
};

}  // namespace masquerade

#endif  // SRC_SERVICES_PROFILE_SERVICE_H_

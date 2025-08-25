// Copyright © 2025 William Huffman

#include "src/services/profile_service.h"

#include "src/database/sqlite_database.h"
#include "src/genproto/profile.pb.h"
#include "src/util/global.h"
#include "src/util/log.h"

namespace masquerade {
namespace {

using masquerade::util::Log::DEBUG;
using masquerade::util::Log::ERROR;
using masquerade::util::Log::INFO;
using std::unordered_map;

}  // namespace

grpc::ServerUnaryReactor* ProfileService::CreateProfile(grpc::CallbackServerContext* context,
                                                        const profile::ProfileCard* request,
                                                        profile::ProfileResponse* response) {
  auto* reactor = context->DefaultReactor();

  if (!(VALID_RPC)) {
    util::log(ERROR, "malformed request");
    reactor->Finish(grpc::Status::CANCELLED);
    return reactor;
  }

  auto db = SqliteDatabase::Connect(global::DB_FILE);
  if (!db.has_value()) {
    util::log(ERROR, "failed to connect to database: {}", db.error().message());
    reactor->Finish(grpc::Status::CANCELLED);
    return reactor;
  }

  // Insert new profile card from request into database.
  reactor->Finish(grpc::Status::OK);
  return reactor;
}

grpc::ServerUnaryReactor* ProfileService::GetProfile(grpc::CallbackServerContext* context,
                                                     const profile::ProfileRequest* request,
                                                     profile::ProfileCard* response) {
  auto* reactor = context->DefaultReactor();

  // Read profile of request.id from db and return it to caller.
  reactor->Finish(grpc::Status::OK);
  return reactor;
}

grpc::ServerUnaryReactor* ProfileService::UpdateProfile(grpc::CallbackServerContext* context,
                                                        const profile::ProfileCard* request,
                                                        profile::ProfileResponse* response) {
  auto* reactor = context->DefaultReactor();

  // Write profile card from request into db.
  reactor->Finish(grpc::Status::OK);
  return reactor;
}

grpc::ServerUnaryReactor* ProfileService::ClearProfile(grpc::CallbackServerContext* context,
                                                       const profile::ProfileRequest* request,
                                                       profile::ProfileResponse* response) {
  auto* reactor = context->DefaultReactor();

  // Search db for profile card with request.id.
  // Delete profile card if it exists.
  // Set profile card for request.id to be default card.
  reactor->Finish(grpc::Status::OK);
  return reactor;
}

}  // namespace masquerade

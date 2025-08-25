// Copyright © 2025 William Huffman

#include "src/services/admin_service.h"

#include <grpcpp/grpcpp.h>

#include <expected>
#include <format>
#include <string>
#include <unordered_map>
#include <vector>

#include "src/database/sqlite_database.h"
#include "src/genproto/admin.pb.h"
#include "src/util/error.h"
#include "src/util/global.h"
#include "src/util/log.h"

namespace masquerade {
namespace {

using masquerade::util::Error;
using masquerade::util::Log::DEBUG;
using masquerade::util::Log::ERROR;
using masquerade::util::Log::INFO;
using std::string;
using std::unordered_map;
using std::vector;

static uint64_t NEXT_ID = 0;  // NOLINT

}  // namespace

grpc::ServerUnaryReactor* AdminService::CreateAccount(grpc::CallbackServerContext* context,
                                                      const admin::Account* request,
                                                      admin::AdminResponse* response) {
  auto* reactor = context->DefaultReactor();

  if (!(VALID_RPC && request->has_first_name() && request->has_last_name() &&
        request->has_phone_number() && request->has_password())) {
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

  const string sql0 = std::format(
      "SELECT ACCOUNT_ID FROM ACCOUNTS"
      "WHERE PHONE_NUMBER={}",
      request->phone_number());

  unordered_map<string, vector<string>> results;

  auto db_result = db->execute(sql0.c_str(), SqliteDatabase::CaptureOutput, &results);

  if (db_result.has_value()) {
    util::log(ERROR, "failed to read database: {}", db_result->message());
    reactor->Finish(grpc::Status::CANCELLED);
    return reactor;
  }
  if (!results.empty()) {
    util::log(INFO, "user with that phone number already exists");
    response->set_status(admin::ADMIN_STATUS_ERR);
    response->set_message("user with that phone number already exists");
    reactor->Finish(grpc::Status::OK);
    return reactor;
  }
  // TODO(whuffman36): sanitize inputs
  // TODO(whuffman36): make user id thread safe

  const uint64_t account_id = NEXT_ID++;
  const string sql1 = std::format(
      "INSERT INTO ACCOUNTS"
      "(ACCOUNT_ID,FIRST_NAME,LAST_NAME,PHONE_NUMBER,PASSWORD)"
      "VALUES ({}, {}, {}, {}, {});",
      account_id, request->first_name(), request->last_name(), request->phone_number(),
      request->password());

  db_result = db->execute(sql1.c_str());
  if (db_result.has_value()) {
    util::log(ERROR, "{}", db_result->message());
    reactor->Finish(grpc::Status::CANCELLED);
    return reactor;
  }

  reactor->Finish(grpc::Status::OK);
  return reactor;
}

grpc::ServerUnaryReactor* AdminService::GetAccount(grpc::CallbackServerContext* context,
                                                   const admin::AccountRequest* request,
                                                   admin::Account* response) {
  auto* reactor = context->DefaultReactor();
  if (!(VALID_RPC && request->has_account_id())) {
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

  const string sql0 = std::format(
      "SELECT * FROM ACCOUNTS"
      "WHERE ACCOUNT_ID={}",
      request->account_id());

  unordered_map<string, vector<string>> results;
  auto db_result = db->execute(sql0.c_str(), SqliteDatabase::CaptureOutput, &results);
  if (db_result.has_value()) {
    util::log(ERROR, "failed to read database: {}", db_result->message());
    reactor->Finish(grpc::Status::CANCELLED);
    return reactor;
  }

  response->set_first_name(results["FIRST_NAME"].front());
  response->set_last_name(results["LAST_NAME"].front());
  response->set_phone_number(results["PHONE_NUMBER"].front());
  response->set_password(results["PASSWORD"].front());

  reactor->Finish(grpc::Status::OK);
  return reactor;
}

grpc::ServerUnaryReactor* AdminService::UpdateAccount(grpc::CallbackServerContext* context,
                                                      const admin::Account* request,
                                                      admin::AdminResponse* response) {
  auto* reactor = context->DefaultReactor();
  if (!(VALID_RPC && request->has_first_name() && request->has_last_name() &&
        request->has_phone_number() && request->has_password())) {
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

  const string sql0 = std::format(
      "SELECT * FROM ACCOUNTS"
      "WHERE ACCOUNT_ID={}",
      request->account_id());

  unordered_map<string, vector<string>> sql_results;

  auto db_result = db->execute(sql0.c_str(), SqliteDatabase::CaptureOutput, &sql_results);

  if (db_result.has_value()) {
    util::log(ERROR, "failed to read database: {}", db_result->message());
    reactor->Finish(grpc::Status::CANCELLED);
    return reactor;
  }
  if (sql_results.empty()) {
    util::log(ERROR, "user {} ({}) does not exist", request->first_name(), request->account_id());
    reactor->Finish(grpc::Status::CANCELLED);
    return reactor;
  }

  const string sql1 = std::format(
      "INSERT INTO ACCOUNTS"
      "(ACCOUNT_ID,FIRST_NAME,LAST_NAME,PHONE_NUMBER,PASSWORD)"
      "VALUES ({}, {}, {}, {}, {});",
      request->account_id(), request->first_name(), request->last_name(), request->phone_number(),
      request->password());

  db_result = db->execute(sql1.c_str());
  if (db_result.has_value()) {
    util::log(ERROR, "{}", db_result->message());
    reactor->Finish(grpc::Status::CANCELLED);
    return reactor;
  }
  reactor->Finish(grpc::Status::OK);
  return reactor;
}

grpc::ServerUnaryReactor* AdminService::DeleteAccount(grpc::CallbackServerContext* context,
                                                      const admin::AccountRequest* request,
                                                      admin::AdminResponse* response) {
  auto* reactor = context->DefaultReactor();
  if (!(VALID_RPC && request->has_account_id())) {
    util::log(ERROR, "malformed request");
    reactor->Finish(grpc::Status::CANCELLED);
    return reactor;
  }

  const auto db = SqliteDatabase::Connect(global::DB_FILE);
  if (!db.has_value()) {
    util::log(ERROR, "failed to connect to database: {}", db.error().message());
    reactor->Finish(grpc::Status::CANCELLED);
    return reactor;
  }

  const string sql0 = std::format(
      "SELECT * FROM ACCOUNTS"
      "WHERE ACCOUNT_ID={}",
      request->account_id());

  unordered_map<string, vector<string>> sql_results;

  auto db_result = db->execute(sql0.c_str(), SqliteDatabase::CaptureOutput, &sql_results);

  if (db_result.has_value()) {
    util::log(ERROR, "failed to read database: {}", db_result->message());
    reactor->Finish(grpc::Status::CANCELLED);
    return reactor;
  }
  if (sql_results.empty()) {
    util::log(ERROR, "user {} does not exist", request->account_id());
    reactor->Finish(grpc::Status::CANCELLED);
    return reactor;
  }

  const string sql1 = std::format(
      "DELETE FROM ACCOUNTS"
      "WHERE ACCOUNT_ID='{}';",
      request->account_id());

  db_result = db->execute(sql1.c_str());
  if (db_result.has_value()) {
    util::log(ERROR, "{}", db_result->message());
    reactor->Finish(grpc::Status::CANCELLED);
    return reactor;
  }

  util::log(INFO, "account deleted {}", request->account_id());
  reactor->Finish(grpc::Status::OK);
  return reactor;
}

}  // namespace masquerade

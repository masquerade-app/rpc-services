// Copyright © 2025 William Huffman

#include "src/services/admin_service.h"

#include <grpcpp/grpcpp.h>

#include <expected>
#include <format>
#include <initializer_list>
#include <iostream>
#include <string>
#include <unordered_map>

#include "src/database/sqlite_database.h"
#include "src/genproto/admin.pb.h"
#include "src/util/error.h"
#include "src/util/global.h"
#include "src/util/log.h"

namespace masquerade {
namespace {

using masquerade::util::Error;
using masquerade::util::Log;
using std::string;

static uint64_t NEXT_ID = 0;  // NOLINT

bool valid_rpc(std::initializer_list<bool> checks) {
  for (const bool check : checks) {
    if (!check) {
      return false;
    }
  }
  return true;
}

}  // namespace

grpc::Status AdminService::CreateAccount(grpc::ServerContext* context,
                                         const admin::Account* request,
                                         admin::AdminResponse* response) {
  if (!valid_rpc({request != nullptr, response != nullptr, context != nullptr,
                  request->has_first_name(), request->has_last_name(),
                  request->has_phone_number(), request->has_password()})) {
    util::log(Log::ERROR, "malformed request");
    return grpc::Status::CANCELLED;
  }

  auto db = SqliteDatabase::Create(global::DB_FILE);
  if (!db.has_value()) {
    util::log(Log::ERROR, "failed to create database: {}",
              db.error().message());
    return grpc::Status::CANCELLED;
  }

  const string sql0 = std::format(
      "SELECT ACCOUNT_ID FROM ACCOUNTS"
      "WHERE PHONE_NUMBER={}",
      request->phone_number());

  std::unordered_map<string, string> results;

  auto db_result =
      db->execute(sql0.c_str(), SqliteDatabase::capture_output, &results);

  if (db_result.has_value()) {
    util::log(Log::ERROR, "failed to read database: {}", db_result->message());
    return grpc::Status::CANCELLED;
  }
  if (!results.empty()) {
    util::log(Log::ERROR, "user with that phone number already exists");
    return grpc::Status::CANCELLED;
  }
  // TODO(whuffman36): sanitize inputs
  // TODO(whuffman36): make user id thread safe

  const uint64_t account_id = NEXT_ID++;
  const string sql1 = std::format(
      "INSERT INTO ACCOUNTS"
      "(ACCOUNT_ID,FIRST_NAME,LAST_NAME,PHONE_NUMBER,PASSWORD)"
      "VALUES ({}, {}, {}, {}, {});",
      account_id, request->first_name(), request->last_name(),
      request->phone_number(), request->password());

  db_result = db->execute(sql1.c_str());
  if (db_result.has_value()) {
    util::log(Log::ERROR, "{}", db_result->message());
    return grpc::Status::CANCELLED;
  }

  return grpc::Status::OK;
}

grpc::Status AdminService::GetAccount(grpc::ServerContext* context,
                                      const admin::AccountRequest* request,
                                      admin::Account* response) {
  if (!valid_rpc({request != nullptr, response != nullptr, context != nullptr,
                  request->has_account_id()})) {
    util::log(Log::ERROR, "malformed request");
    return grpc::Status::CANCELLED;
  }

  auto db = SqliteDatabase::Create(global::DB_FILE);
  if (!db.has_value()) {
    util::log(Log::ERROR, "failed to create database: {}",
              db.error().message());
    return grpc::Status::CANCELLED;
  }

  const string sql0 = std::format(
      "SELECT * FROM ACCOUNTS"
      "WHERE ACCOUNT_ID={}",
      request->account_id());

  std::unordered_map<string, string> results;
  auto db_result =
      db->execute(sql0.c_str(), SqliteDatabase::capture_output, &results);
  if (db_result.has_value()) {
    util::log(Log::ERROR, "failed to read database: {}", db_result->message());
    return grpc::Status::CANCELLED;
  }

  for (auto [key, val] : results) {
    std::cout << "row: " << key << "\t";
    std::cout << "column: " << val << "\n";
  }

  response->set_first_name(results["FIRST_NAME"]);
  response->set_last_name(results["LAST_NAME"]);
  response->set_phone_number(results["PHONE_NUMBER"]);
  response->set_password(results["PASSWORD"]);
  return grpc::Status::OK;
}

grpc::Status AdminService::UpdateAccount(grpc::ServerContext* context,
                                         const admin::Account* request,
                                         admin::AdminResponse* response) {
  if (!valid_rpc({request != nullptr, response != nullptr, context != nullptr,
                  request->has_first_name(), request->has_last_name(),
                  request->has_phone_number(), request->has_password()})) {
    util::log(Log::ERROR, "malformed request");
    return grpc::Status::CANCELLED;
  }

  auto db = SqliteDatabase::Create(global::DB_FILE);
  if (!db.has_value()) {
    util::log(Log::ERROR, "failed to create database: {}",
              db.error().message());
    return grpc::Status::CANCELLED;
  }

  const string sql0 = std::format(
      "SELECT * FROM ACCOUNTS"
      "WHERE ACCOUNT_ID={}",
      request->account_id());

  std::unordered_map<string, string> sql_results;

  auto db_result =
      db->execute(sql0.c_str(), SqliteDatabase::capture_output, &sql_results);

  if (db_result.has_value()) {
    util::log(Log::ERROR, "failed to read database: {}", db_result->message());
    return grpc::Status::CANCELLED;
  }
  if (sql_results.empty()) {
    util::log(Log::ERROR, "user {} ({}) does not exist", request->first_name(),
              request->account_id());
    return grpc::Status::CANCELLED;
  }

  const string sql1 = std::format(
      "INSERT INTO ACCOUNTS"
      "(ACCOUNT_ID,FIRST_NAME,LAST_NAME,PHONE_NUMBER,PASSWORD)"
      "VALUES ({}, {}, {}, {}, {});",
      request->account_id(), request->first_name(), request->last_name(),
      request->phone_number(), request->password());

  db_result = db->execute(sql1.c_str());
  if (db_result.has_value()) {
    util::log(Log::ERROR, "{}", db_result->message());
    return grpc::Status::CANCELLED;
  }
  return grpc::Status::OK;
}

grpc::Status AdminService::DeleteAccount(grpc::ServerContext* context,
                                         const admin::AccountRequest* request,
                                         admin::AdminResponse* response) {
  if (!valid_rpc({request != nullptr, response != nullptr, context != nullptr,
                  request->has_account_id()})) {
    util::log(Log::ERROR, "malformed request");
    return grpc::Status::CANCELLED;
  }

  auto db = SqliteDatabase::Create(global::DB_FILE);
  if (!db.has_value()) {
    util::log(Log::ERROR, "failed to create database: {}",
              db.error().message());
    return grpc::Status::CANCELLED;
  }

  const string sql0 = std::format(
      "SELECT * FROM ACCOUNTS"
      "WHERE ACCOUNT_ID={}",
      request->account_id());

  std::unordered_map<string, string> sql_results;

  auto db_result =
      db->execute(sql0.c_str(), SqliteDatabase::capture_output, &sql_results);

  if (db_result.has_value()) {
    util::log(Log::ERROR, "failed to read database: {}", db_result->message());
    return grpc::Status::CANCELLED;
  }
  if (sql_results.empty()) {
    util::log(Log::ERROR, "user {} does not exist", request->account_id());
    return grpc::Status::CANCELLED;
  }

  const string sql1 = std::format(
      "DELETE FROM ACCOUNTS"
      "WHERE ACCOUNT_ID='{}';",
      request->account_id());

  db_result = db->execute(sql1.c_str());
  if (db_result.has_value()) {
    util::log(Log::ERROR, "{}", db_result->message());
    return grpc::Status::CANCELLED;
  }

  util::log(Log::INFO, "account deleted {}", request->account_id());
  return grpc::Status::OK;
}

}  // namespace masquerade

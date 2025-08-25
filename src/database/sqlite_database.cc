// Copyright © 2025 William Huffman

#include "src/database/sqlite_database.h"

#include <expected>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

extern "C" {
#include "src/database/sqlite/sqlite3.h"
}
#include "src/util/error.h"

namespace masquerade {

namespace {

using std::expected;
using std::optional;
using std::string;
using std::unordered_map;
using std::vector;
using util::Error;

using Callback = int(void*, int, char**, char**);

}  // namespace

expected<SqliteDatabase, Error> SqliteDatabase::Connect(const char* filename) noexcept {
  try {
    return {SqliteDatabase(filename)};
  } catch (std::runtime_error& e) {
    return std::unexpected(Error(e.what()));
  }
}

optional<Error> SqliteDatabase::execute(const char* sql_query, Callback callback,
                                        void* callback_arg) const noexcept {
  if (connection_ == nullptr) {
    return {Error("error: no database connection")};
  }
  char* error_message = nullptr;
  int status = sqlite3_exec(connection_, sql_query, ((callback == nullptr) ? nullptr : callback),
                            callback_arg, &error_message);

  if (status != SQLITE_OK) {
    std::string error;
    if (error_message != nullptr) {
      error = error_message;
      sqlite3_free(error_message);
    }

    return {Error(error)};
  }
  return {std::nullopt};
}

void SqliteDatabase::close() noexcept { sqlite3_close(connection_); }

int SqliteDatabase::CaptureOutput(void* out, int num_columns, char** columns,
                                  char** column_names) noexcept {  // NOLINT
  if (out == nullptr) {
    return 1;
  }
  std::cout << "CaptureOutput() called...\n";
  auto* out_map = static_cast<unordered_map<string, vector<string>>*>(out);

  for (int i = 0; i < num_columns; ++i) {
    if (out_map->contains(column_names[i])) {              // NOLINT
      out_map->at(column_names[i]).push_back(columns[i]);  // NOLINT
    } else {
      out_map->insert(std::make_pair(column_names[i],               // NOLINT
                                     vector<string>{columns[i]}));  // NOLINT
    }
  }

  return 0;
}

optional<Error> SqliteDatabase::InitFromFile(const SqliteDatabase& db,
                                             const char* filename) noexcept {
  if (!std::filesystem::exists(filename)) {
    return {Error("file not found")};
  }
  std::ifstream input(filename);
  if (!input.is_open()) {
    return {Error("failed to open db file")};
  }

  std::string query;
  while (std::getline(input, query)) {
    auto db_result = db.execute(query.c_str());
    if (db_result.has_value()) {
      return db_result;
    }
  }
  return {std::nullopt};
}

SqliteDatabase::SqliteDatabase(SqliteDatabase&& rhs) noexcept
    : filename_(std::move(rhs.filename_)), connection_(rhs.connection_) {
  rhs.connection_ = nullptr;
}

SqliteDatabase& SqliteDatabase::operator=(SqliteDatabase&& rhs) noexcept {
  filename_ = std::move(rhs.filename_);
  connection_ = rhs.connection_;
  rhs.connection_ = nullptr;
  return *this;
}

SqliteDatabase::~SqliteDatabase() noexcept { close(); }

// private:

SqliteDatabase::SqliteDatabase(const char* filename) : filename_(filename), connection_(nullptr) {
  int status = sqlite3_open(filename_.c_str(), &connection_);

  if (status != SQLITE_OK) {
    throw std::runtime_error(sqlite3_errmsg(connection_));
  }
}

}  // namespace masquerade

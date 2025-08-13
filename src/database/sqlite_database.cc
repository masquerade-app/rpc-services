// Copyright © 2025 William Huffman

#include "src/database/sqlite_database.h"

#include <expected>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

extern "C" {
#include "src/database/sqlite/sqlite3.h"
}

namespace masquerade {

namespace {

using Callback = int(void*, int, char**, char**);

}  // namespace

std::expected<SqliteDatabase, std::string> SqliteDatabase::create(
    const char* filename) noexcept {
  try {
    return {SqliteDatabase(filename)};
  } catch (std::runtime_error& e) {
    return std::unexpected(e.what());
  }
}

std::optional<std::string> SqliteDatabase::execute(
    const char* sql_query, const std::function<Callback>& callback,
    void* callback_arg) const noexcept {
  if (connection_ == nullptr) {
    return {"error: no database connection"};
  }
  char* error_message = nullptr;
  int status = sqlite3_exec(
      connection_, sql_query,
      ((callback == nullptr) ? nullptr : callback.target<Callback>()),
      callback_arg, &error_message);

  if (status != SQLITE_OK) {
    std::string error;
    if (error_message != nullptr) {
      error = error_message;
      sqlite3_free(error_message);
    }
    return {error};
  }
  return {std::nullopt};
}

void SqliteDatabase::close() noexcept { sqlite3_close(connection_); }

int SqliteDatabase::capture_output(void* out, int num_columns, char** columns,
                                   char** column_names) {  // NOLINT
  if (out == nullptr) {
    return 1;
  }
  auto* out_map =
      static_cast<std::unordered_map<std::string, std::string>*>(out);

  for (int i = 0; i < num_columns; ++i) {
    out_map->insert(std::make_pair(column_names[i], columns[i]));  // NOLINT
  }

  return 0;
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

SqliteDatabase::SqliteDatabase(const char* filename)
    : filename_(filename), connection_(nullptr) {
  int status = sqlite3_open(filename_.c_str(), &connection_);

  if (status != SQLITE_OK) {
    throw std::runtime_error(sqlite3_errmsg(connection_));
  }
}

const sqlite3* SqliteDatabase::connection() const noexcept {
  return connection_;
}

}  // namespace masquerade

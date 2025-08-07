#include "sqlite_database.h"

extern "C" {
#include "sqlite3.h"
}

#include <expected>
#include <iostream>
#include <stdexcept>

namespace masquerade {

namespace {

using Callback = int(void*, int, char**, char**);

int default_callback(void* arg, int num_columns, char** column_texts,
                     char** column_names) {
  for (int i = 0; i < num_columns; ++i) {
    std::cout << column_names[i] << ": "  // NOLINT
              << (column_texts[i] == nullptr ? "NULL" // NOLINT
                                             : column_texts[i])  // NOLINT
              << "\n";
  }
  return 0;
}

}  // namespace

std::expected<SqliteDatabase, std::string> SqliteDatabase::create(
    const char* filename) noexcept {
  try {
    return SqliteDatabase(filename);
  } catch (std::runtime_error& e) {
    return std::unexpected(e.what());
  }
}

std::expected<int, std::string> SqliteDatabase::execute(
    const char* sql_query, const std::function<Callback>& callback,
    void* callback_arg) const noexcept {
  if (connection_ == nullptr) {
    return std::unexpected("error: no database connection");
  }
  char* error_message = nullptr;
  int status = sqlite3_exec(
      connection_, sql_query,
      ((callback == nullptr) ? default_callback : callback.target<Callback>()),
      callback_arg, &error_message);

  if (status != SQLITE_OK) {
    std::string error;
    if (error_message != nullptr) {
      error = error_message;
      sqlite3_free(error_message);
    }
    return std::unexpected(error);
  }
  return status;
}

void SqliteDatabase::close() noexcept { sqlite3_close(connection_); }

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

SqliteDatabase::~SqliteDatabase() noexcept { sqlite3_close(connection_); }

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

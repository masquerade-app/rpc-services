// Copyright © 2025 William Huffman

#ifndef SRC_DATABASE_SQLITE_DATABASE_H_
#define SRC_DATABASE_SQLITE_DATABASE_H_

#include <expected>
#include <optional>
#include <string>

extern "C" {
#include "src/database/sqlite/sqlite3.h"
}

#include "src/util/error.h"
#include "src/util/move_semantics.h"

namespace masquerade {

struct SqliteDatabase final {
  static std::expected<SqliteDatabase, util::Error> Connect(const char* filename) noexcept;

  std::optional<util::Error> execute(const char* sql_query,
                                     int(callback)(void*, int, char**, char**) = nullptr,
                                     void* callback_arg = nullptr) const noexcept;

  void close() noexcept;

  static int CaptureOutput(void* out, int num_columns, char** columns,
                           char** column_names) noexcept;

  static std::optional<util::Error> InitFromFile(SqliteDatabase& db, const char* filename) noexcept;

  // Movable
  SqliteDatabase(SqliteDatabase&& rhs) noexcept;
  SqliteDatabase& operator=(SqliteDatabase&& rhs) noexcept;

  NOT_COPYABLE(SqliteDatabase)

  ~SqliteDatabase() noexcept;

 private:
  std::string filename_;
  sqlite3* connection_;

  explicit SqliteDatabase(const char* filename);
};

}  // namespace masquerade

#endif  // SRC_DATABASE_SQLITE_DATABASE_H_

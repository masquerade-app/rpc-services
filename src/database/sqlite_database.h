// Copyright © 2025 William Huffman

#ifndef SRC_DATABASE_SQLITE_DATABASE_H_
#define SRC_DATABASE_SQLITE_DATABASE_H_

#include <expected>
#include <functional>
#include <optional>
#include <string>

extern "C" {
#include "src/database/sqlite/sqlite3.h"
}
#include "src/util/move_semantics.h"

namespace masquerade {

struct SqliteDatabase final {
  static std::expected<SqliteDatabase, std::string> create(
      const char* filename) noexcept;

  std::optional<std::string> execute(
      const char* sql_query,
      const std::function<int(void*, int, char**, char**)>& callback = nullptr,
      void* callback_arg = nullptr) const noexcept;

  void close() noexcept;

  static int capture_output(void* out, int num_columns, char** columns,
                            char** column_names);

  // Movable
  SqliteDatabase(SqliteDatabase&& rhs) noexcept;
  SqliteDatabase& operator=(SqliteDatabase&& rhs) noexcept;

  NOT_COPYABLE(SqliteDatabase)

  ~SqliteDatabase() noexcept;

 private:
  std::string filename_;
  sqlite3* connection_;

  explicit SqliteDatabase(const char* filename);
  [[nodiscard]] const sqlite3* connection() const noexcept;
};

}  // namespace masquerade

#endif  // SRC_DATABASE_SQLITE_DATABASE_H_

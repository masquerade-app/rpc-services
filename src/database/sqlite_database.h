#ifndef MASQUERADE_RPC_SERVICES_DATABASE_DATABASE_H
#define MASQUERADE_RPC_SERVICES_DATABASE_DATABASE_H

#include "database.h"

extern "C" {
#include "sqlite3.h"
}

#include <expected>
#include <functional>
#include <string>

namespace masquerade {

class SqliteDatabase {
  std::string filename_;
  sqlite3* connection_;

 public:
  static std::expected<SqliteDatabase, std::string> create(
      const char* filename) noexcept;

  std::expected<int, std::string> execute(
      const char* sql_query,
      const std::function<int(void*, int, char**, char**)>& callback = nullptr,
      void* callback_arg = nullptr) const noexcept;

  void close() noexcept;

  // Movable
  SqliteDatabase(SqliteDatabase&& rhs) noexcept;
  SqliteDatabase& operator=(SqliteDatabase&& rhs) noexcept;

  // Not copyable
  SqliteDatabase(const SqliteDatabase& rhs) = delete;
  SqliteDatabase& operator=(const SqliteDatabase& rhs) = delete;

  ~SqliteDatabase() noexcept;

 private:
  explicit SqliteDatabase(const char* filename);
  [[nodiscard]] const sqlite3* connection() const noexcept;
};

}  // namespace masquerade

#endif

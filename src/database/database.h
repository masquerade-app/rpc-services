#ifndef MASQUERADE_RPC_SERVICES_DATABASE_DATABASE_H
#define MASQUERADE_RPC_SERVICES_DATABASE_DATABASE_H

#include <optional>
#include <string>

namespace masquerade {

class Database {
 public:
  Database() = default;
  virtual std::optional<std::string> read() = 0;
  virtual std::optional<std::string> write() = 0;
  virtual ~Database() = default;
  explicit Database(const Database& rhs) = delete;
  explicit Database(Database&& rhs) = delete;
  Database& operator=(const Database& rhs) = delete;
  Database& operator=(Database&& rhs) = delete;
};

}  // namespace masquerade

#endif

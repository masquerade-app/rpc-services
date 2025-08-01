#ifndef MASQUERADE_DATABASE_H
#define MASQUERADE_DATABASE_H

#include <optional>
#include <string>

namespace masquerade {

class Database {
 protected:
  Database() = default;
  virtual std::optional<std::string> read() = 0;
  virtual std::optional<std::string> write() = 0;
  ~Database() = default;

 public:
  explicit Database(const Database& rhs) = delete;
  explicit Database(Database&& rhs) = delete;
  Database operator=(const Database& rhs) = delete;
  Database operator=(Database&& rhs) = delete;
};

}  // namespace masquerade

#endif

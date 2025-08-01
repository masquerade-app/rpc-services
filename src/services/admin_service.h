#ifndef MASQUERADE_ADMIN_SERVICE_H
#define MASQUERADE_ADMIN_SERVICE_H

#include <cstdint>
#include <expected>
#include <optional>
#include <string>

namespace masquerade {

class Account final {
  uint64_t account_id_{};
  std::string phone_number_;
  std::string first_name_;
  std::string last_name_;
  std::string password_;

 public:
  static std::expected<Account, std::string> create(std::string phone_number, std::string first_name, std::string last_name, std::string password) noexcept;
  [[nodiscard]] uint64_t account_id() const noexcept;
  [[nodiscard]] std::string phone_number() const noexcept;
  [[nodiscard]] std::string first_name() const noexcept;
  [[nodiscard]] std::string last_name() const noexcept;
  std::optional<std::string> update_first_name(std::string_view new_name) noexcept;
  std::optional<std::string> update_last_name(std::string_view new_name) noexcept;
  std::optional<std::string> update_password(std::string_view new_name) noexcept;

  // Not copyable or movable
  explicit Account() = delete;
  explicit Account(const Account& rhs) = delete;
  explicit Account(Account&& rhs) = delete;
  Account operator=(const Account& rhs) = delete;
  Account operator=(Account&& rhs) = delete;
  ~Account() = default;

 private:
  Account(std::string phone_number, std::string first_name, std::string last_name, std::string password) noexcept;
};

}  // namespace masquerade

#endif

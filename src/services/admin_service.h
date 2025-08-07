#ifndef MASQUERADE_RPC_SERVICES_SERVICES_ADMIN_SERVICE_H
#define MASQUERADE_RPC_SERVICES_SERVICES_ADMIN_SERVICE_H

#include <cstdint>
#include <string>
#include <string_view>

namespace masquerade {

class Account final {
  uint64_t account_id_;
  std::string first_name_;
  std::string last_name_;
  std::string phone_number_;
  std::string password_;

public:
  static Account create() noexcept;

  [[nodiscard]] inline uint64_t account_id() const noexcept;

  [[nodiscard]] inline std::string_view first_name() const noexcept;

  [[nodiscard]] inline std::string_view last_name() const noexcept;

  [[nodiscard]] inline std::string_view phone_number() const noexcept;

  Account& set_first_name(std::string_view name) noexcept;

  Account& set_last_name(std::string_view name) noexcept;

  Account& set_phone_number(std::string_view phone_number) noexcept;

  Account& set_password(std::string_view password) noexcept;

  ~Account() = default;

  // Movable
  Account(Account&& rhs) = default;
  Account& operator=(Account&& rhs) = default;

  // Not copyable
  Account(const Account& rhs) = delete;
  Account& operator=(const Account& rhs) = delete;

private:
  Account();
};

}  // namespace masquerade

#endif

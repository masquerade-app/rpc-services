// Copyright © 2025 William Huffman

#include "services/admin_service.h"

#include <expected>
#include <string>

namespace masquerade {

Account Account::create() noexcept {
  Account account;
  return account;
}

[[nodiscard]] inline uint64_t Account::account_id() const noexcept {
  return account_id_;
}

[[nodiscard]] inline std::string_view Account::first_name() const noexcept {
  return first_name_;
}

[[nodiscard]] inline std::string_view Account::last_name() const noexcept {
  return last_name_;
}

[[nodiscard]] inline std::string_view Account::phone_number() const noexcept {
  return phone_number_;
}

Account& Account::set_first_name(std::string_view name) noexcept {
  first_name_ = name;
  return *this;
}

Account& Account::set_last_name(std::string_view name) noexcept {
  last_name_ = name;
  return *this;
}

Account& Account::set_phone_number(std::string_view phone_number) noexcept {
  phone_number_ = phone_number;
  return *this;
}

Account& Account::set_password(std::string_view password) noexcept {
  password_ = password;
  return *this;
}

// private:
Account::Account() : account_id_(0) {}

}  // namespace masquerade

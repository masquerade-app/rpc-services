// Copyright © 2025 William Huffman

#ifndef SRC_DATABASE_SQL_FORMATTER_H_
#define SRC_DATABASE_SQL_FORMATTER_H_

#include <initializer_list>
#include <string>
#include <string_view>

namespace masquerade {

struct SqlFormatter {
  inline std::string& insert_into(std::string_view table,
                                  std::string_view table_schema) noexcept;

  inline std::string& select(std::string_view field) noexcept;

  inline std::string& from(std::string_view table) noexcept;

  inline std::string& where(std::string_view field) noexcept;

  inline consteval std::string& equals() noexcept;

  inline std::string& value(std::string_view value) noexcept;

  std::string& values(std::initializer_list<std::string_view>) noexcept;

  // Trailing underscore due to delete being a keyword in C++.
  inline consteval std::string& delete_() noexcept;

  [[nodiscard]] inline const std::string& format() noexcept;

 private:
  std::string sql_;
};
}  // namespace masquerade

#endif  // SRC_DATABASE_SQL_FORMATTER_H_

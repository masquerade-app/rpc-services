// Copyright © 2025 William Huffman

#ifndef SRC_DATABASE_SQL_FORMATTER_H_
#define SRC_DATABASE_SQL_FORMATTER_H_

#include <initializer_list>
#include <string>
#include <string_view>

namespace masquerade {

// TODO(whuffman36): Inline SqlFormatter class.
struct SqlFormatter final {
  static SqlFormatter Query() noexcept;

  SqlFormatter& insert_into(std::string_view table, std::string_view table_schema) noexcept;

  SqlFormatter& select(std::string_view field) noexcept;

  SqlFormatter& from(std::string_view table) noexcept;

  SqlFormatter& where(std::string_view field) noexcept;

  SqlFormatter& equals() noexcept;

  SqlFormatter& value(std::string_view value) noexcept;

  SqlFormatter& values(std::initializer_list<std::string_view>) noexcept;

  // Trailing underscore due to delete being a keyword in C++.
  SqlFormatter& delete_() noexcept;

  [[nodiscard]] const std::string& format() noexcept;

 private:
  std::string sql_;
};
}  // namespace masquerade

#endif  // SRC_DATABASE_SQL_FORMATTER_H_

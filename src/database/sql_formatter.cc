// Copyright © 2025 William Huffman

#include "src/database/sql_formatter.h"

#include <format>
#include <initializer_list>
#include <string>
#include <string_view>

using std::initializer_list;
using std::string;
using std::string_view;

namespace masquerade {

SqlFormatter SqlFormatter::Query() noexcept { return SqlFormatter{}; }

SqlFormatter& SqlFormatter::insert_into(string_view table, string_view table_schema) noexcept {
  sql_ = std::format("INSERT INTO {} {}", table, table_schema);
  return *this;
}

SqlFormatter& SqlFormatter::select(string_view field) noexcept {
  sql_ = std::format("SELECT {}", field);
  return *this;
}

SqlFormatter& SqlFormatter::from(string_view table) noexcept {
  sql_.append(std::format(" FROM {}", table));
  return *this;
}

SqlFormatter& SqlFormatter::where(string_view field) noexcept {
  sql_.append(std::format(" WHERE {}", field));
  return *this;
}

SqlFormatter& SqlFormatter::equals() noexcept {
  sql_.append("=");
  return *this;
}

SqlFormatter& SqlFormatter::value(string_view value) noexcept {
  sql_.append(std::format("{}", value));
  return *this;
}

SqlFormatter& SqlFormatter::values(initializer_list<string_view> values) noexcept {
  sql_.append(" VALUES (");
  for (const auto& value : values) {
    sql_.append(std::format("{},", value));
  }
  sql_.back() = ')';
  return *this;
}

// Trailing underscore due to delete being a keyword in C++.
SqlFormatter& SqlFormatter::delete_() noexcept {
  sql_ = "DELETE";
  return *this;
}

[[nodiscard]] const string& SqlFormatter::format() noexcept {
  sql_.append(";");
  return sql_;
}

}  // namespace masquerade

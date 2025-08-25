// Copyright © 2025 William Huffman

#include "src/database/sql_formatter.h"

#include <gtest/gtest.h>

#include <string>
#include <string_view>

using std::string;
using std::string_view;

TEST(SqlFormatterTest, Insert) {
  const auto query = masquerade::SqlFormatter::Query()
                         .insert_into("table", "(NAME, AGE)")
                         .values({"'Liam'", "25"})
                         .format();

  constexpr string_view expected = "INSERT INTO table (NAME, AGE) VALUES ('Liam',25);";

  ASSERT_EQ(query, expected);
}

TEST(SqlFormatterTest, Select) {
  const auto query = masquerade::SqlFormatter::Query()
                         .select("AGE")
                         .from("table")
                         .where("NAME")
                         .equals()
                         .value("'Liam'")
                         .format();

  constexpr string_view expected = "SELECT AGE FROM table WHERE NAME='Liam';";

  ASSERT_EQ(query, expected);
}

TEST(SqlFormatterTest, Delete) {
  const auto query = masquerade::SqlFormatter::Query()
                         .delete_()
                         .from("table")
                         .where("AGE")
                         .equals()
                         .value("25")
                         .format();

  constexpr string_view expected = "DELETE FROM table WHERE AGE=25;";

  ASSERT_EQ(query, expected);
}

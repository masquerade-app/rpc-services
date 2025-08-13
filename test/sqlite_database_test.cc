// Copyright © 2025 William Huffman

#include "src/database/sqlite_database.h"

#include <gtest/gtest.h>

#include <filesystem>
#include <optional>
#include <utility>

static const auto TEST_DATABASE_PATH =
    std::filesystem::current_path().concat("/test.db");

class SqliteDatabaseTest : public testing::Test {
 protected:
  std::optional<masquerade::SqliteDatabase> db;

  SqliteDatabaseTest() : db(std::nullopt) {}

  void SetUp() override {
    auto db_result =
        masquerade::SqliteDatabase::create(TEST_DATABASE_PATH.c_str());
    ASSERT_TRUE(db_result.has_value());

    db = std::move(db_result.value());
    ASSERT_TRUE(db.has_value());
  }

  void TearDown() override {
    db->close();
    std::filesystem::remove(TEST_DATABASE_PATH);
    db = std::nullopt;
  }
};

TEST_F(SqliteDatabaseTest, Execute) {
  const char *sql =
      "CREATE TABLE IF NOT EXISTS TEST_TABLE ("
      "NAME VARCHAR PRIMARY KEY NOT NULL,"
      "AGE INT NOT NULL"
      ");";

  auto result = db->execute(sql);
  EXPECT_FALSE(result.has_value());

  sql =
      "INSERT INTO TEST_TABLE (NAME, AGE) "
      "VALUES ('Liam', 25);"
      "INSERT INTO TEST_TABLE (NAME, AGE) "
      "VALUES ('Nicholas', 27);";

  result = db->execute(sql);
  EXPECT_FALSE(result.has_value());

  sql = "SELECT * FROM TEST_TABLE";

  result = db->execute(sql);
  EXPECT_FALSE(result.has_value());
}

// Copyright © 2025 William Huffman

#include "src/database/sqlite_database.h"

#include <gtest/gtest.h>

#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace {

using std::optional;
using std::string;
using std::unordered_map;
using std::vector;

const auto TEST_DATABASE_ASSETS = std::filesystem::current_path()  // NOLINT
                                      .parent_path()               // NOLINT
                                      .parent_path()               // NOLINT
                                      .concat("/test/assets/");    // NOLINT

const auto TEST_DATABASE = TEST_DATABASE_ASSETS / "test.db";
const auto TEST_INIT_FILE = TEST_DATABASE_ASSETS / "init.sql";

}  // namespace

class SqliteDatabaseTest : public testing::Test {
 protected:
  optional<masquerade::SqliteDatabase> db;

  SqliteDatabaseTest() : db(std::nullopt) {}

  void SetUp() override {
    auto db_result = masquerade::SqliteDatabase::Connect(TEST_DATABASE.c_str());
    ASSERT_TRUE(db_result.has_value());

    db.emplace(std::move(*db_result));
    ASSERT_TRUE(db.has_value());
  }

  void TearDown() override {
    db->close();
    std::filesystem::remove(TEST_DATABASE);
    db = std::nullopt;
  }
};

TEST_F(SqliteDatabaseTest, Execute) {
  const char* sql =
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

TEST_F(SqliteDatabaseTest, InitFromFile) {
  const auto init_result = masquerade::SqliteDatabase::InitFromFile(*db, TEST_INIT_FILE.c_str());
  ASSERT_FALSE(init_result.has_value());

  unordered_map<string, vector<string>> column_map;
  const char* sql = "SELECT * FROM USERS";

  auto result = db->execute(sql, masquerade::SqliteDatabase::CaptureOutput, &column_map);
  ASSERT_FALSE(result.has_value());

  EXPECT_EQ(column_map["FIRST_NAME"][0], "Liam");
  EXPECT_EQ(column_map["FIRST_NAME"][1], "Nicholas");
  EXPECT_EQ(column_map["LAST_NAME"][0], "Huffman");
  EXPECT_EQ(column_map["AGE"][0], "25");
  EXPECT_EQ(column_map["FIRST_NAME"][3], "Austin");
  EXPECT_EQ(column_map["AGE"][3], "28");
}

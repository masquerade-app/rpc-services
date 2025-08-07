#include "../src/database/sqlite_database.h"

#include <gtest/gtest.h>

TEST(SqliteDatabaseTest, Execute) {
  auto db_result = masquerade::SqliteDatabase::create("test/test.db");
  ASSERT_TRUE(db_result.has_value());

  masquerade::SqliteDatabase db = std::move(db_result.value());

  const char* sql =
      "CREATE TABLE IF NOT EXISTS TEST_TABLE ("
      "NAME VARCHAR PRIMARY KEY NOT NULL,"
      "AGE INT NOT NULL"
      ");";

  auto result = db.execute(sql);
  EXPECT_TRUE(result.has_value());

  sql =
      "INSERT INTO TEST_TABLE (NAME, AGE) "
      "VALUES ('Liam', 25);"
      "INSERT INTO TEST_TABLE (NAME, AGE) "
      "VALUES ('Nicholas', 27);";

  result = db.execute(sql);
  EXPECT_TRUE(result.has_value());

  sql = "SELECT * FROM TEST_TABLE";

  result = db.execute(sql);
  EXPECT_TRUE(result.has_value());
}

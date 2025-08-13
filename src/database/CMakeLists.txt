add_library(
  sqlite
  STATIC
  sqlite/sqlite3.c
)

add_library(
  sql_formatter
  STATIC
  sql_formatter.cc
)

add_library(
  database
  STATIC
  sqlite_database.cc
)

target_link_libraries(
  database
  PUBLIC
  sqlite
  sql_formatter
)
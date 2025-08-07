add_library(
  sqlite
  STATIC
  sqlite3.c
)

add_library(
  database
  STATIC
  sqlite_database.cc
)

target_include_directories(
  database
  PUBLIC
  ..
)

target_link_libraries(
  database
  PUBLIC
  sqlite
)
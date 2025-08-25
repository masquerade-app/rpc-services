enable_testing()
set(CMAKE_CXX_CLANG_TIDY "")

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        52eb8108c5bdec04579160ae17225d66034bd723 # v1.17.0
)
FetchContent_MakeAvailable(googletest)

add_executable(
  sqlite_database_test
  sqlite_database_test.cc
)

target_link_libraries(
  sqlite_database_test
  PRIVATE
  GTest::gtest_main
  database
)

add_executable(
  sql_formatter_test
  sql_formatter_test.cc
)

target_link_libraries(
  sql_formatter_test
  PRIVATE
  GTest::gtest_main
  sql_formatter
)

include(GoogleTest)
gtest_discover_tests(sqlite_database_test)
gtest_discover_tests(sql_formatter_test)
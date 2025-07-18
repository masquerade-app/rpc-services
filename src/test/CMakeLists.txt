enable_testing()

add_executable(
  hello_test
  hello_test.cc
)

target_include_directories(
  hello_test
  PRIVATE
  build/_deps/googletest-src/googlemock/include
  build/_deps/googletest-src/googletest/include
)

target_link_libraries(
  hello_test
  PRIVATE
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)
enable_testing()

find_package(GTest CONFIG REQUIRED)

add_executable(
  hello_test
  hello_test.cc
)

target_include_directories(
  hello_test
  PRIVATE
  /deps/grpc/third_party/googletest/googletest/include
  /deps/grpc/third_party/googletest/googlemock/include
)

target_link_libraries(
  hello_test
  PRIVATE
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)
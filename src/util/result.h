// Copyright © 2025 William Huffman

#ifndef SRC_UTIL_RESULT_H_
#define SRC_UTIL_RESULT_H_

#include <utility>
#include <variant>

#include "src/util/error.h"
#include "src/util/move_semantics.h"

namespace masquerade::util {

template <typename T>
struct Result final {
  explicit Result(T&& value) : data_(std::in_place_t(), value) {}
  explicit Result(Error&& error) : data_(std::in_place_t(), error) {}

  bool is_ok() { return data_.index() == 0; }
  bool is_err() { return data_.index() == 1; }

  T&& unwrap() { return std::forward<T&&>(std::get<0>(data_)); }
  Error&& err() { return std::forward<Error&&>(std::get<1>(data_)); }

  T&& unwrap_or(T&& default_value) {
    if (is_ok()) {
      return unwrap();
    }
    return default_value;
  }

  MOVEABLE(Result)
  NOT_COPYABLE(Result)
  Result() = delete;
  ~Result() = default;

 private:
  std::variant<T, Error> data_;
};

}  // namespace masquerade::util

#endif  // SRC_UTIL_RESULT_H_

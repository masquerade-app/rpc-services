// Copyright © 2025 William Huffman

#ifndef SRC_UTIL_ERROR_H_
#define SRC_UTIL_ERROR_H_

#include <string>
#include <string_view>

#include "src/util/move_semantics.h"

namespace masquerade::util {

struct Error final {
  explicit Error(const char* message) : message_(message) {}
  explicit Error(std::string_view message) : message_(message) {}

  std::string_view message() { return std::string_view(message_); }

  MOVEABLE(Error)
  NOT_COPYABLE(Error)
  ~Error() = default;

 private:
  std::string message_;
};

}  // namespace masquerade::util

#endif  // SRC_UTIL_ERROR_H_

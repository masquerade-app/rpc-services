// Copyright © 2025 William Huffman

#ifndef SRC_UTIL_LOG_H_
#define SRC_UTIL_LOG_H_

#include <format>
#include <iostream>
#include <string_view>

namespace masquerade::util {

enum class Log { DEBUG, INFO, ERROR };

template <typename... Args>
void log(Log level, std::string_view message, Args&&... args) {
  switch (level) {
    case Log::DEBUG:
      std::cout << "[DEBUG]: "
                << std::vformat(message, std::make_format_args(args...))
                << "\n";
      break;
    case Log::INFO:
      std::cout << "[INFO]: "
                << std::vformat(message, std::make_format_args(args...))
                << "\n";
      break;
    case Log::ERROR:
      std::cerr << "[ERROR]: "
                << std::vformat(message, std::make_format_args(args...))
                << "\n";
      break;
  }
}

}  // namespace masquerade::util

#endif  // SRC_UTIL_LOG_H_

// Copyright © 2025 William Huffman

#ifndef SRC_UTIL_GLOBAL_H_
#define SRC_UTIL_GLOBAL_H_

#include <filesystem>

namespace masquerade::global {

const char* DB_FILE = std::filesystem::current_path()   // NOLINT
                          .concat("/database/main.db")  // NOLINT
                          .c_str();                     // NOLINT

#define VALID_RPC request != nullptr&& response != nullptr&& context != nullptr

}  // namespace masquerade::global

#endif  // SRC_UTIL_GLOBAL_H_

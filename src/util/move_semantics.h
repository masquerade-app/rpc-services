// Copyright © 2025 William Huffman

#ifndef SRC_UTIL_MOVE_SEMANTICS_H_
#define SRC_UTIL_MOVE_SEMANTICS_H_

#define COPYABLE(Struct)               \
  Struct(const Struct& rhs) = default; \
  Struct& operator=(const Struct& rhs) = default;

#define MOVEABLE(Struct)          \
  Struct(Struct&& rhs) = default; \
  Struct& operator=(Struct&& rhs) = default;

#define NOT_COPYABLE(Struct)          \
  Struct(const Struct& rhs) = delete; \
  Struct& operator=(const Struct& rhs) = delete;

#define NOT_MOVEABLE(Struct)     \
  Struct(Struct&& rhs) = delete; \
  Struct& operator=(Struct&& rhs) = delete;

#endif  // SRC_UTIL_MOVE_SEMANTICS_H_

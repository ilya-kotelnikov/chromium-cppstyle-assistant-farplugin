////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#include <cstring>

namespace std {

void* memset(void* dst, int val, size_t size) {
  static_assert(sizeof(unsigned long) == 4);

  const unsigned long v1 = val & 0xff;
  const unsigned long v4 =
      (v1 == 0) ? 0UL : ((v1 << 24) | (v1 << 16) | (v1 << 8) | v1);
  unsigned long* p_v4 = reinterpret_cast<unsigned long*>(dst);
  unsigned long* p_v4_end = p_v4 + size / 4;
  while (p_v4 != p_v4_end)
    *p_v4++ = v4;

  unsigned char* p_v1 = reinterpret_cast<unsigned char*>(p_v4);
  unsigned char* p_v1_end = reinterpret_cast<unsigned char*>(dst) + size;
  while (p_v1 != p_v1_end)
    *p_v1++ = v1;

  return dst;
}

}  // namespace std

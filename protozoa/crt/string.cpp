////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#include <string.h>

#include <windows.h>

extern "C" {

int _wcsicmp(const wchar_t* lhs, const wchar_t* rhs) {
  return ::lstrcmpiW(lhs, rhs);
}

wchar_t* wcscpy(wchar_t* dst, const wchar_t* src) {
  return ::lstrcpyW(dst, src);
}

int wcslen(const wchar_t* src) {
  return ::lstrlen(src);
}

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

int memcmp(const void* lhs, const void* rhs, size_t size) {
  static_assert(sizeof(unsigned long) == 4);

  const unsigned long* pl_v4 = reinterpret_cast<const unsigned long*>(lhs);
  const unsigned long* pl_v4_end = pl_v4 + size / 4;
  const unsigned long* pr_v4 = reinterpret_cast<const unsigned long*>(rhs);
  for (; pl_v4 != pl_v4_end; ++pl_v4, ++pr_v4) {
    if (*pl_v4 != *pr_v4)
      break;  // compare the same pair by bytes to have correct result.
  }

  const unsigned char* pl_v1 = reinterpret_cast<const unsigned char*>(pl_v4);
  const unsigned char* pl_v1_end =
      reinterpret_cast<const unsigned char*>(lhs) + size;
  const unsigned char* pr_v1 = reinterpret_cast<const unsigned char*>(pr_v4);
  for (; pl_v1 != pl_v1_end; ++pl_v1, ++pr_v1) {
    const unsigned char d = *pl_v1 - *pr_v1;
    if (d != 0)
      return d;
  }

  return 0;
}

void* memcpy(void* dst, const void* src, size_t size) {
  static_assert(sizeof(unsigned long) == 4);

  unsigned long* pd_v4 = reinterpret_cast<unsigned long*>(dst);
  unsigned long* pd_v4_end = pd_v4 + size / 4;
  const unsigned long* ps_v4 = reinterpret_cast<const unsigned long*>(src);
  for (; pd_v4 != pd_v4_end; ++pd_v4, ++ps_v4)
    *pd_v4 = *ps_v4;

  unsigned char* pd_v1 = reinterpret_cast<unsigned char*>(pd_v4);
  unsigned char* pd_v1_end = reinterpret_cast<unsigned char*>(dst) + size;
  const unsigned char* ps_v1 = reinterpret_cast<const unsigned char*>(ps_v4);
  for (; pd_v1 != pd_v1_end; ++pd_v1, ++ps_v1)
    *pd_v1 = *ps_v1;

  return dst;
}

}  // extern "C"


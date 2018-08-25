////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#include <stdint.h>

#include <windows.h>

void* operator new(size_t n) noexcept(false) {
  return ::HeapAlloc(::GetProcessHeap(), 0, n);
}

void operator delete(void* p) throw() {
#ifndef NDEBUG
  ::HeapValidate(::GetProcessHeap(), 0, p);
#endif
  ::HeapFree(::GetProcessHeap(), 0, p);
}

void operator delete(void* p, size_t) throw() {
  operator delete(p);
}

void* operator new[](size_t n) noexcept(false) {
  return operator new(n);
}

void operator delete[](void* p) throw() {
  operator delete(p);
}

void operator delete[](void* p, size_t) throw() {
  operator delete(p);
}

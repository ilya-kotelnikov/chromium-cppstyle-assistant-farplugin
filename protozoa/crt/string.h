////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#pragma once

#include <stdint.h>

extern "C" {

int _wcsicmp(const wchar_t* lhs, const wchar_t* rhs);
wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);

// Note: the implementation of the routine assumes that |dst| is 4-bytes aligned
// which should be generally true if it was allocated by typical operator new()
// or malloc() or on stack.
void* memset(void* dst, int val, size_t size);

// Note: the implementation of the routine assumes that |lhs| and |rhs| are
// 4-bytes aligned should be generally true if they were allocated by typical
// operator new() or malloc() or on stack.
int memcmp(const void* lhs, const void* rhs, size_t size);

// Note: the implementation of the routine assumes that |dst| and |src| are
// 4-bytes aligned should be generally true if they were allocated by typical
// operator new() or malloc() or on stack.
void* memcpy(void* dst, const void* src, size_t size);

}  // extern "C"

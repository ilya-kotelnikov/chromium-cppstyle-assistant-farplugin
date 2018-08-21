////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#pragma once

#include <windows.h>  // for COLORREF.

#ifdef NDEBUG
#define assert(expr)
#else
#define assert(expr) do { if (!(expr)) \
                            ::MessageBoxA(nullptr, #expr, "assert", \
                                          MB_OK | MB_ICONSTOP); } while(0)
#endif

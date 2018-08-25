////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#pragma once

namespace protozoa {
namespace crt {

//
// Do call this routine as the first thing at the app start: it initializes
// internal structures required for statics defined in functions.
//
// Note: global statics are not supported at all - they are left uninitialized.
//
void init();

//
// Do call this routine as the last thing at the app termination: it calls
// functions registered by atexit() routine.
//
void term();

}  // namespace crt
}  // namespace protozoa

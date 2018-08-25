////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#include <windows.h>

#ifdef MINICRT
#include "protozoa/crt/crt.h"
#endif

extern "C" BOOL WINAPI DllMain(HINSTANCE module_handle, DWORD reason, LPVOID) {
  switch (reason) {
    case DLL_PROCESS_ATTACH:
#ifdef MINICRT
      protozoa::crt::init();
#endif
      break;

    case DLL_PROCESS_DETACH:
#ifdef MINICRT
      protozoa::crt::term();
#endif
      break;
  }
  return TRUE;
}

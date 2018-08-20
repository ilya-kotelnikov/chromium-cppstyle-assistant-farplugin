////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#include <initguid.h>  // due this header the guids are defined in this module.
#include "globals.hpp"

#include <cassert>

#include <Plugin.hpp>

namespace cc_assistant {

PluginStartupInfo& g_psi(const PluginStartupInfo* psi_to_init_from) {
  static PluginStartupInfo psi;
  static FarStandardFunctions fsf;

  bool the_first_call = false;
  static bool initialized_threadsafely __attribute__((unused)) = ([&]() {
    // The first call to this function MUST pass the argument provided by Far.
    assert(psi_to_init_from != nullptr);

    psi = (*psi_to_init_from);
    fsf = (*psi_to_init_from->FSF);
    psi.FSF = &fsf;

    the_first_call = true;
    return true;
  })();

  // Any call coming after the first one MUST pass null.
  assert(the_first_call || psi_to_init_from == nullptr);
  return psi;
}

}  // namespace cc_assistant

////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#include <initguid.h>  // due this header the guids are defined in this module.
#include "globals.hpp"

namespace cc_assistant {

////////////////////////////////////////////////////////////////////////////////
// Define plugin global objects.
//
PluginStartupInfo g_psi;
FarStandardFunctions g_fsf;
ConfigSettings g_opt;

}  // namespace cc_assistant

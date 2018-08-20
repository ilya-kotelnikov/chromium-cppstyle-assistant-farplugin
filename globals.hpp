////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#include <guiddef.h>

struct PluginStartupInfo;

namespace cc_assistant {

////////////////////////////////////////////////////////////////////////////////
// Declare plugin guids.
//
// {61062d79-f1eb-4afc-a3c3-aa2c0cd6375b}
DEFINE_GUID(g_plugin_guid, 0x61062d79, 0xf1eb, 0x4afc, 0xa3, 0xc3,
                           0xaa, 0x2c, 0x0c, 0xd6, 0x37, 0x5b);

// {019bba44-7e1e-4207-ba7c-ea860f57681d}
DEFINE_GUID(g_menu_guid, 0x019bba44, 0x7e1e, 0x4207, 0xba, 0x7c,
                         0xea, 0x86, 0x0f, 0x57, 0x68, 0x1d);

// {898b7610-48c5-4f9b-9b72-bc1d066eccd9}
DEFINE_GUID(g_config_dialog_guid, 0x898b7610, 0x48c5, 0x4f9b, 0x9b, 0x72,
                                  0xbc, 0x1d, 0x06, 0x6e, 0xcc, 0xd9);

////////////////////////////////////////////////////////////////////////////////
// Declare the main plugin accessor to Far API.
//
// Notes:
//   1. Instead of a raw object access a function is provided to check the
// object is initialized before any usage.
//   2. Once do call the function with the Far API accessor object provided by
// SetStartupInfoW() - it will initialize this plugin instance's copy of the
// object (a local copy is required as the object passed by Far is deleted as
// soon as SetStartupInfoW() call ends).
//   3. Also the function checks that a call with the 'init' argument is the
// first call to this function during this plugin instance lifetime.
//
PluginStartupInfo& g_psi(const PluginStartupInfo* psi_to_init_from =nullptr);

}  // namespace cc_assistant

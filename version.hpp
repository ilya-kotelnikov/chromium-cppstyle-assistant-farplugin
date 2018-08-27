////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#include <FarVersion.hpp>

#define PLUGIN_BUILD 3
#define PLUGIN_NAME L"Chromium CppStyle Assistant"
#define PLUGIN_DESC L"Chromium C++ style assistant for FarManager"
#define PLUGIN_AUTHOR L"ilya.kotelnikov@gmail.com"
#define PLUGIN_FILENAME L"Chromium-CppStyle-Assistant.dll"
#define PLUGIN_VERSION MAKEFARVERSION(FARMANAGERVERSION_MAJOR, \
                                      FARMANAGERVERSION_MINOR, \
                                      FARMANAGERVERSION_BUILD, \
                                      PLUGIN_BUILD, VS_RELEASE)

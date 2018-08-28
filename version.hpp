////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#include <FarVersion.hpp>

#define PLUGIN_BUILD 4
#define PLUGIN_NAME L"CCAssistant"
#define PLUGIN_DESC L"Chromium C++ codestyle assistant"
#define PLUGIN_AUTHOR L"ilya.kotelnikov@gmail.com"
#define PLUGIN_FILENAME L"CCAssistant.dll"
#define PLUGIN_VERSION MAKEFARVERSION(FARMANAGERVERSION_MAJOR, \
                                      FARMANAGERVERSION_MINOR, \
                                      FARMANAGERVERSION_BUILD, \
                                      PLUGIN_BUILD, VS_RELEASE)

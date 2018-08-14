////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#pragma once

////////////////////////////////////////////////////////////////////////////////
// Names and default values for plugin settings.
//
extern const wchar_t kHighlightLineLimitColumnSettingName[];
extern const wchar_t kHighlightLineLimitColumnIndexSettingName[];
extern const wchar_t kDetectWhitespacesAtLineEndsSettingName[];

constexpr int kHighlightLineLimitColumnSettingDefault = 1;
constexpr unsigned int kHighlightLineLimitColumnIndexSettingDefault = 80U;
constexpr int kDetectWhitespacesAtLineEndsSettingDefault = 1;

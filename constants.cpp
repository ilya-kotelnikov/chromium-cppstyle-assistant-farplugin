////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#include "constants.hpp"

namespace cc_assistant {

////////////////////////////////////////////////////////////////////////////////
// Help topics.
//
// Note: keep consistent with "distrib/Options*.hlf" files' contents.
// 
const wchar_t kMenuHelpTopic[] = L"Contents";
const wchar_t kConfigHelpTopic[] = L"Configure";

////////////////////////////////////////////////////////////////////////////////
// Names and default values for plugin settings.
//
const wchar_t kFileMasksSettingName[] = L"FileMasks";
const wchar_t kHighlightLineLimitColumnEnabledSettingName[] =
    L"HighlightLineLimitColumnEnabled";
const wchar_t kHighlightLineLimitColumnIndexSettingName[] =
    L"HighlightLineLimitColumnIndex";
const wchar_t kHighlightLineLimitColumnForecolorSettingName[] =
    L"HighlightLineLimitColumnForecolor";
const wchar_t kHighlightLineLimitColumnBackcolorSettingName[] =
    L"HighlightLineLimitColumnBackcolor";
const wchar_t kHighlightLineLimitColumnBackcolorIfTabsSettingName[] =
    L"HighlightLineLimitColumnBackcolorIfTabs";

const wchar_t kFileMasksSettingDefault[] = L"*.c;*.cc;*.cpp;*.h;*.hh;*.hpp;";

}  // namespace cc_assistant

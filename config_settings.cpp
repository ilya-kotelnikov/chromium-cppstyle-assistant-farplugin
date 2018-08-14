////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#include "config_settings.hpp"

#include <PluginSettings.hpp>

#include "constants.hpp"

namespace cc_assistant {

ConfigSettings::ConfigSettings()
    : highlight_linelimit_column(
          kHighlightLineLimitColumnSettingDefault),
      highlight_linelimit_column_index(
          kHighlightLineLimitColumnIndexSettingDefault),
      detect_whitespaces_at_line_ends(
          kDetectWhitespacesAtLineEndsSettingDefault) {
}

void ConfigSettings::LoadFromFarStorage(/*const*/ PluginSettings& storage) {
  highlight_linelimit_column =
      storage.Get(0, kHighlightLineLimitColumnSettingName,
                     kHighlightLineLimitColumnSettingDefault);
  highlight_linelimit_column_index =
      storage.Get(0, kHighlightLineLimitColumnIndexSettingName,
                     kHighlightLineLimitColumnIndexSettingDefault);
  detect_whitespaces_at_line_ends =
      storage.Get(0, kDetectWhitespacesAtLineEndsSettingName,
                     kDetectWhitespacesAtLineEndsSettingDefault);
}

void ConfigSettings::SaveToFarStorage(PluginSettings* storage) const {
  storage->Set(0, kHighlightLineLimitColumnSettingName,
                  highlight_linelimit_column);
  storage->Set(0, kHighlightLineLimitColumnIndexSettingName,
                  highlight_linelimit_column_index);
  storage->Set(0, kDetectWhitespacesAtLineEndsSettingName,
                  detect_whitespaces_at_line_ends);
}

}  // namespace cc_assistant

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

ConfigSettings::
HighlightLineLimitColumnSettings::HighlightLineLimitColumnSettings()
    : enabled(kHighlightLineLimitColumnEnabledSettingDefault),
      column_index(kHighlightLineLimitColumnIndexSettingDefault),
      forecolor(kHighlightLineLimitColumnForecolorSettingDefault),
      backcolor(kHighlightLineLimitColumnBackcolorSettingDefault),
      backcolor_if_tabs(kHighlightLineLimitColumnBackcolorIfTabsSettingDefault),
      file_masks(kHighlightLineLimitColumnFileMasksSettingDefault) {
}

ConfigSettings::ConfigSettings() : highlight_linelimit_column_settings() {
}

void ConfigSettings::LoadFromFarStorage(/*const*/ PluginSettings& storage) {
  auto& hlcs = highlight_linelimit_column_settings;
  hlcs.enabled =
      storage.Get(0, kHighlightLineLimitColumnEnabledSettingName,
                     kHighlightLineLimitColumnEnabledSettingDefault);
  hlcs.column_index =
      storage.Get(0, kHighlightLineLimitColumnIndexSettingName,
                     kHighlightLineLimitColumnIndexSettingDefault);
  hlcs.forecolor =
      storage.Get(0, kHighlightLineLimitColumnForecolorSettingName,
                     kHighlightLineLimitColumnForecolorSettingDefault);
  hlcs.backcolor =
      storage.Get(0, kHighlightLineLimitColumnBackcolorSettingName,
                     kHighlightLineLimitColumnBackcolorSettingDefault);
  hlcs.backcolor_if_tabs =
      storage.Get(0, kHighlightLineLimitColumnBackcolorIfTabsSettingName,
                     kHighlightLineLimitColumnBackcolorIfTabsSettingDefault);
  hlcs.file_masks =
      storage.Get(0, kHighlightLineLimitColumnFileMasksSettingName,
                     kHighlightLineLimitColumnFileMasksSettingDefault);
}

void ConfigSettings::SaveToFarStorage(PluginSettings* storage) const {
  auto& hlcs = highlight_linelimit_column_settings;
  storage->Set(0, kHighlightLineLimitColumnEnabledSettingName, hlcs.enabled);
  storage->Set(0, kHighlightLineLimitColumnIndexSettingName, hlcs.column_index);
  storage->Set(0, kHighlightLineLimitColumnForecolorSettingName,
                  hlcs.forecolor);
  storage->Set(0, kHighlightLineLimitColumnBackcolorSettingName,
                  hlcs.backcolor);
  storage->Set(0, kHighlightLineLimitColumnBackcolorIfTabsSettingName,
                  hlcs.backcolor_if_tabs);
  storage->Set(0, kHighlightLineLimitColumnFileMasksSettingName,
                  hlcs.file_masks.c_str());
}

}  // namespace cc_assistant

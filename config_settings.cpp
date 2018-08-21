////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#include "config_settings.hpp"

#include <PluginSettings.hpp>

#include "constants.hpp"
#include "globals.hpp"

namespace cc_assistant {

ConfigSettings::
HighlightLineLimitColumnSettings::HighlightLineLimitColumnSettings()
    : enabled(kHighlightLineLimitColumnEnabledSettingDefault),
      column_index(kHighlightLineLimitColumnIndexSettingDefault),
      forecolor(kHighlightLineLimitColumnForecolorSettingDefault),
      backcolor(kHighlightLineLimitColumnBackcolorSettingDefault),
      backcolor_if_tabs(
          kHighlightLineLimitColumnBackcolorIfTabsSettingDefault) {
}

// static
ConfigSettings* ConfigSettings::GetInstance() {
  static ConfigSettings instance;
  return &instance;
}

void ConfigSettings::ReLoadFromFarStorage() {
  PluginSettings storage(g_plugin_guid, g_psi().SettingsControl);

  file_masks = storage.Get(0, kFileMasksSettingName, kFileMasksSettingDefault);

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
}

void ConfigSettings::SaveToFarStorage() const {
  PluginSettings storage(g_plugin_guid, g_psi().SettingsControl);

  storage.Set(0, kFileMasksSettingName, file_masks.c_str());

  auto& hlcs = highlight_linelimit_column_settings;
  storage.Set(0, kHighlightLineLimitColumnEnabledSettingName, hlcs.enabled);
  storage.Set(0, kHighlightLineLimitColumnIndexSettingName, hlcs.column_index);
  storage.Set(0, kHighlightLineLimitColumnForecolorSettingName, hlcs.forecolor);
  storage.Set(0, kHighlightLineLimitColumnBackcolorSettingName, hlcs.backcolor);
  storage.Set(0, kHighlightLineLimitColumnBackcolorIfTabsSettingName,
                 hlcs.backcolor_if_tabs);
}

ConfigSettings::ConfigSettings() : file_masks(kFileMasksSettingDefault),
                                   highlight_linelimit_column_settings() {
  ReLoadFromFarStorage();
}

ConfigSettings::~ConfigSettings() {
}

}  // namespace cc_assistant

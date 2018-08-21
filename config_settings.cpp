////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#include "config_settings.hpp"

#include <PluginSettings.hpp>

#include "constants.hpp"
#include "editor_filename_match_cache.hpp"
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

  UpdateEditorFileNameMatchCache();
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

  UpdateEditorFileNameMatchCache();
}

EditorFileNameMatchCache* ConfigSettings::editor_filename_match_cache() {
  if (!editor_filename_match_cache_ && IsEditorFileNameMatchCacheUsable()) {
    editor_filename_match_cache_.reset(new EditorFileNameMatchCache());
    editor_filename_match_cache_->SetFileMasks(file_masks);
  }

  return editor_filename_match_cache_.get();
}

bool ConfigSettings::editor_filename_match_default() const {
  // Empty file masks match everything.
  return file_masks.empty();
}

ConfigSettings::ConfigSettings() : file_masks(kFileMasksSettingDefault),
                                   highlight_linelimit_column_settings() {
  ReLoadFromFarStorage();
}

ConfigSettings::~ConfigSettings() {
}

void ConfigSettings::UpdateEditorFileNameMatchCache() const {
  if (!IsEditorFileNameMatchCacheUsable())
    editor_filename_match_cache_.reset();
  else if (editor_filename_match_cache_)
    editor_filename_match_cache_->SetFileMasks(file_masks);
}

bool ConfigSettings::IsEditorFileNameMatchCacheUsable() const {
  // The cache makes sense only if 'file masks' setting is not empty and
  // at least one feature is on.
  return !file_masks.empty() && highlight_linelimit_column_settings.enabled;
}

}  // namespace cc_assistant

////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#pragma once

#include <memory>
#include <string>

#include <windows.h>  // for COLORREF.

namespace cc_assistant {

class EditorFileNameMatchCache;

class ConfigSettings {
 public:
  // "Highlight line-limit column" feature settings.
  struct HighlightLineLimitColumnSettings {
    int enabled;
    unsigned int column_index;
    COLORREF forecolor, backcolor, backcolor_if_tabs;

    HighlightLineLimitColumnSettings();
  };

  std::wstring file_masks;
  HighlightLineLimitColumnSettings highlight_linelimit_column_settings; 

  static ConfigSettings* GetInstance();

  void ReLoadFromFarStorage();
  void SaveToFarStorage() const;

  // The function returns null if all the features are off in the settings or
  // file masks are empty (the object is useless in these cases, so it is
  // deleted not to waist memory).
  EditorFileNameMatchCache* editor_filename_match_cache();

  // Plugin match function should return this function result (default match
  // value) in case of null editor_filename_match_cache(). It may be true or
  // false depending on the settings and internal logic.
  bool editor_filename_match_default() const;

 private:
  ConfigSettings();
  ~ConfigSettings();

  void UpdateEditorFileNameMatchCache() const;
  bool IsEditorFileNameMatchCacheUsable() const;

  mutable
      std::unique_ptr<EditorFileNameMatchCache> editor_filename_match_cache_;
};

}  // namespace cc_assistant

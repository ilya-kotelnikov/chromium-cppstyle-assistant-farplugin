////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#pragma once

#include <string>

#include <windows.h>  // for COLORREF.

namespace cc_assistant {

struct ConfigSettings {
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

 private:
  ConfigSettings();
  ~ConfigSettings();
};

}  // namespace cc_assistant

////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#pragma once

class PluginSettings;

namespace cc_assistant {

struct ConfigSettings {
  // Highlight or not line-limit column.
  int highlight_linelimit_column;

  // Line-limit column number to highlight.
  unsigned int highlight_linelimit_column_index;

  // Highlight or not whitespaces at line ends.
  int detect_whitespaces_at_line_ends;

  ConfigSettings();

  void LoadFromFarStorage(/*const*/ PluginSettings& storage);
  void SaveToFarStorage(PluginSettings* storage) const;
};

}  // namespace cc_assistant

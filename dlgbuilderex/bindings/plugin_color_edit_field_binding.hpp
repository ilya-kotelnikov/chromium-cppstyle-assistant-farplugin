////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#pragma once

#include <windows.h>  // for COLORREF.

#include "dlgbuilderex/bindings/plugin_generic_edit_field_binding.hpp"

namespace dlgbuilderex {

constexpr size_t kColorEditFieldValueWidth = 6;  // "RRGGBB" hex.

class PluginColorEditFieldBinding :
    public PluginGenericEditFieldBinding<COLORREF> {
 public:
  PluginColorEditFieldBinding(const PluginStartupInfo& plugin_startup_info,
                              HANDLE* dialog_handle,
                              int item_id,
                              COLORREF* option_var);

  // DialogAPIBindingEx overrides:
  const wchar_t* GenerateEditFieldMaskOnce(int field_width) override;
  const wchar_t* GetInitialValueAsStringData() const override;
  void SetResultValueFromStringData(const wchar_t* data) override;
  void UpdateInitialValue() override;

 private:
  wchar_t initial_value_as_string_[kColorEditFieldValueWidth + 1];
};

}  // namespace dlgbuilderex

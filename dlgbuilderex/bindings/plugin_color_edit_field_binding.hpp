////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#pragma once

#include <windows.h>  // for COLORREF.

#include "dlgbuilderex/bindings/plugin_generic_edit_field_binding.hpp"

constexpr size_t kColorEditFieldValueWidth = 6;  // "RRGGBB" hex.
extern const wchar_t kColorEditFieldValueMask[];  // "HHHHHH"

class PluginColorEditFieldBinding :
    public PluginGenericEditFieldBinding<COLORREF> {
 public:
  PluginColorEditFieldBinding(const PluginStartupInfo& plugin_startup_info,
                              HANDLE* dialog_handle,
                              int item_id,
                              COLORREF* option_var);

  // DialogAPIBindingGetSetVarMix overrides:
  const wchar_t* GetInitialValueAsStringData() const  override;
  void SetResultValueFromStringData(const wchar_t* data) const override;

 private:
  wchar_t initial_value_as_string_[kColorEditFieldValueWidth + 1];
};

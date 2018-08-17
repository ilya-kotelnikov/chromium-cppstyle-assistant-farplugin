////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#pragma once

#include <string>

#include "dlgbuilderex/bindings/plugin_generic_edit_field_binding.hpp"

class PluginStringEditFieldBinding :
    public PluginGenericEditFieldBinding<std::wstring> {
 public:
  PluginStringEditFieldBinding(const PluginStartupInfo& plugin_startup_info,
                               HANDLE* dialog_handle,
                               int item_id,
                               std::wstring* option_var)
      : PluginGenericEditFieldBinding<std::wstring>(
            plugin_startup_info, dialog_handle, item_id, option_var) {
  }

  // DialogAPIBindingGetSetVarMix overrides:
  const wchar_t* GetInitialValueAsStringData() const override {
    return option_var_->c_str();
  }

  void SetResultValueFromStringData(const wchar_t* data) const override {
    option_var_->assign(data);
  }
};

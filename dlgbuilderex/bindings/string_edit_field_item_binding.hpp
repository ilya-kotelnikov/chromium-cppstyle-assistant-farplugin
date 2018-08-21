////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#pragma once

#include <string>

#include "dlgbuilderex/bindings/generic_edit_field_item_binding.hpp"

namespace dlgbuilderex {

class StringEditFieldItemBinding :
    public GenericEditFieldItemBinding<std::wstring> {
 public:
  StringEditFieldItemBinding(const PluginStartupInfo& plugin_startup_info,
                             HANDLE* dialog_handle,
                             int item_id,
                             std::wstring* option_var)
      : GenericEditFieldItemBinding<std::wstring>(
            plugin_startup_info, dialog_handle, item_id, option_var) {
    UpdateInitialValue();
  }

  void UpdateInitialValue() override {
    initial_value_as_string_.assign(*option_var_);
  }

  const wchar_t* GetInitialValueAsStringData() const override {
    return initial_value_as_string_.c_str();
  }

  void SetResultValueFromStringData(const wchar_t* data) override {
    option_var_->assign(data);
  }

 private:
  std::wstring initial_value_as_string_;
};

}  // namespace dlgbuilderex

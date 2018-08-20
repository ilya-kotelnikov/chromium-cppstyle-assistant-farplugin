////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#pragma once

#include "dlgbuilderex/bindings/plugin_generic_edit_field_binding.hpp"

namespace dlgbuilderex {

// max(unsigned int) = 4294967295 -> max 10 digits.
constexpr int kUIntValueMaxDigitsCount = 10;

class PluginUIntEditFieldBinding :
    public PluginGenericEditFieldBinding<unsigned int> {
 public:
  PluginUIntEditFieldBinding(const PluginStartupInfo& plugin_startup_info,
                             HANDLE* dialog_handle,
                             int item_id,
                             unsigned int* option_var);

  // DialogAPIBindingEx overrides:
  const wchar_t* GenerateEditFieldMaskOnce(int field_width) override;
  const wchar_t* GetInitialValueAsStringData() const override;
  void SetResultValueFromStringData(const wchar_t* data) override;
  void UpdateInitialValue() override;

 private:
  wchar_t initial_value_buffer_[kUIntValueMaxDigitsCount + 1];
  wchar_t field_mask_buffer_[kUIntValueMaxDigitsCount + 1];
};

}  // namespace dlgbuilderex

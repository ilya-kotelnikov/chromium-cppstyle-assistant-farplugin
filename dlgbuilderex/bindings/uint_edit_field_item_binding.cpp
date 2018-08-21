////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#include "dlgbuilderex/bindings/uint_edit_field_item_binding.hpp"

#include "globals.hpp"

namespace dlgbuilderex {

UIntEditFieldItemBinding::UIntEditFieldItemBinding(
    const PluginStartupInfo& plugin_startup_info,
    HANDLE* dialog_handle, int item_id, unsigned int* option_var)
    : GenericEditFieldItemBinding<unsigned int>(
          plugin_startup_info, dialog_handle, item_id, option_var) {
  UpdateInitialValue();
}

const wchar_t* UIntEditFieldItemBinding::GenerateEditFieldMaskOnce(
    int field_width) {
  // Ensure input will not go out of the buffer.
  const int mask_width =
      (field_width < kUIntValueMaxDigitsCount) ? field_width
                                               : kUIntValueMaxDigitsCount;
  for (int i = 0; i < mask_width; ++i)
    field_mask_buffer_[i] = L'9';
  field_mask_buffer_[mask_width] = 0;

  return field_mask_buffer_;
}

void UIntEditFieldItemBinding::UpdateInitialValue() {
  cc_assistant::g_psi().FSF->sprintf(
      initial_value_buffer_, L"%u", *option_var_);
}

const wchar_t* UIntEditFieldItemBinding::GetInitialValueAsStringData() const {
  return initial_value_buffer_;
}

void UIntEditFieldItemBinding::SetResultValueFromStringData(
    const wchar_t* data) {
  *option_var_ = cc_assistant::g_psi().FSF->atoi(data);
}

}  // namespace dlgbuilderex

////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#pragma once

#include "dlgbuilderex/bindings/item_binding.hpp"

namespace dlgbuilderex {

class CheckboxItemBinding : public ItemBinding {
 public:
  CheckboxItemBinding(const PluginStartupInfo& plugin_startup_info,
                      HANDLE* dialog_handle,
                      int item_id,
                      int* option_var,
                      int checked_state_value_mask = 0)
      : ItemBinding(plugin_startup_info, dialog_handle, item_id),
        option_var_(option_var),
        checked_state_value_mask_(checked_state_value_mask) {
  }

  void SaveValue(FarDialogItem const* item, int radio_group_index) override {
    const int checked = static_cast<int>(
        DialogAPIBinding::Info.SendDlgMessage(
            *DialogAPIBinding::DialogHandle, DM_GETCHECK, DialogAPIBinding::ID,
            nullptr));

    if (checked_state_value_mask_) {
      if (checked)
        (*option_var_) |= checked_state_value_mask_;
      else
        (*option_var_) &= ~checked_state_value_mask_;
    } else {
      (*option_var_) = checked;
    }
  }

  void UpdateItemInitialValue(FarDialogItem* item) override {
    if (checked_state_value_mask_)
      item->Selected = ((*option_var_ & checked_state_value_mask_) != 0);
    else
      item->Selected = *option_var_;
  }

 private:
   int* option_var_;
   int checked_state_value_mask_;
};

}  // namespace dlgbuilderex

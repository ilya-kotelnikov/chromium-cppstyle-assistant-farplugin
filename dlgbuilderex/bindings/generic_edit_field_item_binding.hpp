////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#pragma once

#include "dlgbuilderex/bindings/item_binding.hpp"

namespace dlgbuilderex {

// Define additional interface methods for all of our edit field bindings to
// enable generic algorithms.
//
class EditFieldItemBinding : public ItemBinding {
 public:
  EditFieldItemBinding(const PluginStartupInfo& plugin_startup_info,
                       HANDLE* dialog_handle, int item_id)
      : ItemBinding(plugin_startup_info, dialog_handle, item_id) {
  }

  virtual const wchar_t* GenerateEditFieldMaskOnce(int field_width) {
    return nullptr;  // no mask by default.
  }

  virtual void UpdateInitialValue() = 0;
  virtual const wchar_t* GetInitialValueAsStringData() const = 0;
  virtual void SetResultValueFromStringData(const wchar_t* data) = 0;
};

// A generic implementation of a binding between a variable of some type and its
// edit field in a dialog.
//
template<class T>
class GenericEditFieldItemBinding : public EditFieldItemBinding {
 public:
  using OptionVarType = T;

  GenericEditFieldItemBinding(const PluginStartupInfo& plugin_startup_info,
                              HANDLE* dialog_handle,
                              int item_id,
                              T* option_var)
      : EditFieldItemBinding(plugin_startup_info, dialog_handle, item_id),
        option_var_(option_var) {
  }

  void SaveValue(FarDialogItem const* item, int radio_group_index) override {
    const wchar_t* current_edit_field_string_data =
        reinterpret_cast<const wchar_t*>(
            DialogAPIBinding::Info.SendDlgMessage(
                *DialogAPIBinding::DialogHandle, DM_GETCONSTTEXTPTR,
                DialogAPIBinding::ID, nullptr));

    SetResultValueFromStringData(current_edit_field_string_data);
  }

  void UpdateItemInitialValue(FarDialogItem* item) override {
    UpdateInitialValue();
    item->Data = GetInitialValueAsStringData();
  }

  // The following methods must be defined in final edit field binding class:
  //
  // void UpdateInitialValue() override;
  // const wchar_t* GetInitialValueAsStringData() const override;
  // void SetResultValueFromStringData(const wchar_t* data) override;

 protected:
  T* option_var_;
};

}  // namespace dlgbuilderex

////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#pragma once

#include <DlgBuilder.hpp>

// Define additional interface methods for our edit field bindings to enable
// generic algorithms.
//
class DialogAPIBindingEx : public DialogAPIBinding {
 public:
  DialogAPIBindingEx(const PluginStartupInfo& plugin_startup_info,
                     HANDLE* dialog_handle, int item_id)
      : DialogAPIBinding(plugin_startup_info, dialog_handle, item_id) {
  }

  virtual const wchar_t* GetInitialValueAsStringData() const = 0;
  virtual void SetResultValueFromStringData(const wchar_t* data) const = 0;
};

// A generic implementation of a binding between a variable of some type and its
// edit field in a dialog.
//
template<class T>
class PluginGenericEditFieldBinding : public DialogAPIBindingEx {
 public:
  using OptionVarType = T;

  PluginGenericEditFieldBinding(const PluginStartupInfo& plugin_startup_info,
                                HANDLE* dialog_handle,
                                int item_id,
                                T* option_var)
      : DialogAPIBindingEx(plugin_startup_info, dialog_handle, item_id),
        option_var_(option_var) {
  }

  void SaveValue(FarDialogItem* item, int radio_group_index) override {
    const wchar_t* current_edit_field_string_data =
        reinterpret_cast<const wchar_t*>(
            DialogAPIBinding::Info.SendDlgMessage(
                *DialogAPIBinding::DialogHandle, DM_GETCONSTTEXTPTR,
                DialogAPIBinding::ID, nullptr));

    SetResultValueFromStringData(current_edit_field_string_data);
  }

  // The following methods must be defined in final binding subclass:
  //
  // const wchar_t* GetInitialValueAsStringData() const  override;
  // void SetResultValueFromStringData(const wchar_t* data) const override;

 protected:
  T* option_var_;
};

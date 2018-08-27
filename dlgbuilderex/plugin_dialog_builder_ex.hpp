////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#pragma once

#include <string>

#include <windows.h>  // for COLORREF.

#include <DlgBuilder.hpp>

#include "dlgbuilderex/bindings/generic_edit_field_item_binding.hpp"

namespace dlgbuilderex {

//
// An extended version of the Far API PluginDialogBuilder.
//
// Provides:
// 1. Specific UI control types based on standard ones:
//      - edit field to type a color;
//      - edit field to type file masks.
// 2. Several convenience methods to manipulate the controls.
//

class PluginDialogBuilderEx : public PluginDialogBuilder {
 public:
  PluginDialogBuilderEx(const PluginStartupInfo& plugin_startup_info,
                        const GUID& plugin_guid,
                        const GUID& dialog_guid,
                        const wchar_t* title_msg,
                        const wchar_t* help_topic);
  PluginDialogBuilderEx(const PluginStartupInfo& plugin_startup_info,
                        const GUID& plugin_guid,
                        const GUID& dialog_guid,
                        int title_msg_id,
                        const wchar_t* help_topic);
  ~PluginDialogBuilderEx() override;

  // Add a label with centered text.
  FarDialogItem* AddCenteredText(const wchar_t* label);

  // Add a standard checkbox for an int variable (our version of the original
  // checkbox enables generic algorithms to manipulate the item).
  FarDialogItem* AddCheckboxItem(const wchar_t* label,
                                 int* option_var,
                                 int checked_state_value_mask = 0,
                                 bool three_state = false);

  // Add an edit for a color variable represented as "RRGGBB" in hex.
  FarDialogItem* AddColorEditFieldItem(COLORREF* option_var);

  // Add an edit for a string variable.
  FarDialogItem* AddStringEditFieldItem(std::wstring* option_var,
                                        int edit_field_width);

  // Add an edit for a unsigned int variable (our version of the original uint
  // field enables generic algorithms to manipulate the item).
  FarDialogItem* AddUIntEditFieldItem(unsigned int* option_var,
                                      int edit_field_width);

  // Update item's initial content/state to correspond to current value of its
  // bound option variable.
  // Note: only an item added by Add*Item() function declared in this class may
  // be passed as the argument! No check is possible as RTTI is disabled so
  // another kind of an item passed here as the argument will just crash the
  // app.
  void UpdateItemInitialValue(FarDialogItem* item);

 protected:
  template<class BindingType>
  FarDialogItem* AddGenericEditFieldItem(
      FARDIALOGITEMTYPES field_type,
      int field_width,
      typename BindingType::OptionVarType* option_var) {
    return DoAddGenericEditFieldItem(
        field_type, field_width,
        new BindingType(PluginDialogBuilder::Info,
                        &(PluginDialogBuilder::DialogHandle),
                        PluginDialogBuilder::m_DialogItemsCount, option_var));
  }

 private:
  FarDialogItem* DoAddGenericEditFieldItem(
      FARDIALOGITEMTYPES field_type, int field_width,
      EditFieldItemBinding* new_edit_field_item_binding);
};

}  // namespace dlgbuilderex

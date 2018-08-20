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

#include "dlgbuilderex/bindings/plugin_generic_edit_field_binding.hpp"

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

  // Add a separator and return it for fine-tuning (standard AddSeparator()
  // returns void).
  FarDialogItem* AddSeparatorEx(const wchar_t* text = L"");
  FarDialogItem* AddSeparatorEx(int text_msg_id);

  // Add an edit for a color variable represented as "RRGGBB" in hex.
  FarDialogItem* AddColorEditField(COLORREF* option_var);

  // Add an edit for a string variable.
  FarDialogItem* AddStringEditField(std::wstring* option_var,
                                    int edit_field_width);

  // Add an edit for a unsigned int variable (our version of the original uint
  // field enables generic algorithms to manipulate with the field).
  FarDialogItem* AddUIntEditField(unsigned int* option_var,
                                  int edit_field_width) override;

  // Update item's Data property to the current value of its bound option
  // variable.
  // Note: only an item added by an Add*() function declared in this class may
  // be passed as the argument! No check is possible as RTTI is disabled so
  // another kind of an item passed here as the argument will just crash the
  // app.
  void UpdateItemInitialValue(FarDialogItem* item);

 protected:
  template<class BindingType>
  FarDialogItem* AddGenericEditFieldForOptionVar(
      FARDIALOGITEMTYPES field_type,
      int field_width,
      typename BindingType::OptionVarType* option_var) {
    return DoAddGenericEditFieldForOptionVar(field_type, field_width,
        new BindingType(PluginDialogBuilder::Info,
                        &(PluginDialogBuilder::DialogHandle),
                        PluginDialogBuilder::m_DialogItemsCount, option_var));
  }

 private:
  FarDialogItem* DoAddGenericEditFieldForOptionVar(
      FARDIALOGITEMTYPES field_type,
      int field_width,
      DialogAPIBindingEx* new_item_to_option_var_binding);
};

}  // namespace dlgbuilderex

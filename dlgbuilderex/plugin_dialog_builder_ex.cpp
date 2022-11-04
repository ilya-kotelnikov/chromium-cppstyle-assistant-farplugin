////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#include "plugin_dialog_builder_ex.hpp"

#include "dlgbuilderex/bindings/checkbox_item_binding.hpp"
#include "dlgbuilderex/bindings/color_edit_field_item_binding.hpp"
#include "dlgbuilderex/bindings/generic_edit_field_item_binding.hpp"
#include "dlgbuilderex/bindings/string_edit_field_item_binding.hpp"
#include "dlgbuilderex/bindings/uint_edit_field_item_binding.hpp"

namespace dlgbuilderex {

PluginDialogBuilderEx::PluginDialogBuilderEx(
    const PluginStartupInfo& plugin_startup_info,
    const GUID& plugin_guid,
    const GUID& dialog_guid,
    const wchar_t* title_msg,
    const wchar_t* help_topic)
    : PluginDialogBuilder(plugin_startup_info, plugin_guid, dialog_guid,
                          title_msg, help_topic) {
}

PluginDialogBuilderEx::PluginDialogBuilderEx(
    const PluginStartupInfo& plugin_startup_info,
    const GUID& plugin_guid,
    const GUID& dialog_guid,
    int title_msg_id,
    const wchar_t* help_topic)
    : PluginDialogBuilder(plugin_startup_info, plugin_guid, dialog_guid,
                          title_msg_id, help_topic) {
}

PluginDialogBuilderEx::~PluginDialogBuilderEx() = default;

FarDialogItem* PluginDialogBuilderEx::AddCenteredText(const wchar_t* label) {
  FarDialogItem* item = PluginDialogBuilder::AddText(label);
  item->Flags |= DIF_CENTERTEXT;
  return item;
}

FarDialogItem* PluginDialogBuilderEx::AddCheckboxItem(
    const wchar_t* label, int* option_var, int checked_state_value_mask,
    bool three_state) {
  ItemBinding* new_checkbox_item_binding =
        new CheckboxItemBinding(PluginDialogBuilder::Info,
                                &(PluginDialogBuilder::DialogHandle),
                                PluginDialogBuilder::m_DialogItemsCount,
                                option_var,
                                checked_state_value_mask);

  FarDialogItem* item = PluginDialogBuilder::AddDialogItem(DI_CHECKBOX, label);

  new_checkbox_item_binding->UpdateItemInitialValue(item);

  if (three_state && checked_state_value_mask == 0)
    item->Flags |= DIF_3STATE;

  PluginDialogBuilder::SetNextY(item);
  item->X2 = item->X1 + PluginDialogBuilder::ItemWidth(*item);

  PluginDialogBuilder::SetLastItemBinding(new_checkbox_item_binding);
  return item;
}

FarDialogItem* PluginDialogBuilderEx::AddColorEditFieldItem(
    COLORREF* option_var) {
  return AddGenericEditFieldItem<ColorEditFieldItemBinding>(
             DI_FIXEDIT, kColorEditFieldValueWidth, option_var);
}

FarDialogItem* PluginDialogBuilderEx::AddStringEditFieldItem(
    std::wstring* option_var, int edit_field_width) {
  return AddGenericEditFieldItem<StringEditFieldItemBinding>(
             DI_EDIT, edit_field_width, option_var);
}

FarDialogItem* PluginDialogBuilderEx::AddUIntEditFieldItem(
    unsigned int* option_var, int edit_field_width) {
  return AddGenericEditFieldItem<UIntEditFieldItemBinding>(
             DI_FIXEDIT, edit_field_width, option_var);
}

void PluginDialogBuilderEx::UpdateItemInitialValue(FarDialogItem* item) {
  ItemBinding* binding =
      static_cast<ItemBinding*>(PluginDialogBuilder::FindBinding(item));
  if (binding)
    binding->UpdateItemInitialValue(item);
}

FarDialogItem* PluginDialogBuilderEx::DoAddGenericEditFieldItem(
    FARDIALOGITEMTYPES field_type,
    int field_width,
    EditFieldItemBinding* new_edit_field_item_binding) {
  FarDialogItem* item = PluginDialogBuilder::AddDialogItem(field_type, nullptr);

  new_edit_field_item_binding->UpdateItemInitialValue(item);

  item->Mask =
      new_edit_field_item_binding->GenerateEditFieldMaskOnce(field_width);
  if (item->Mask)
    item->Flags |= DIF_MASKEDIT;

  PluginDialogBuilder::SetNextY(item);
  item->X2 = item->X1 + field_width - 1;

  PluginDialogBuilder::SetLastItemBinding(new_edit_field_item_binding);
  return item;
}

}  // namespace dlgbuilderex

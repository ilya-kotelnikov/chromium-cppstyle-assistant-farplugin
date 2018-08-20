////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#include "plugin_dialog_builder_ex.hpp"

#include "dlgbuilderex/bindings/plugin_color_edit_field_binding.hpp"
#include "dlgbuilderex/bindings/plugin_generic_edit_field_binding.hpp"
#include "dlgbuilderex/bindings/plugin_string_edit_field_binding.hpp"
#include "dlgbuilderex/bindings/plugin_uint_edit_field_binding.hpp"

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

PluginDialogBuilderEx::~PluginDialogBuilderEx() {
}

FarDialogItem* PluginDialogBuilderEx::AddSeparatorEx(const wchar_t* text) {
  FarDialogItem* item = PluginDialogBuilder::AddDialogItem(DI_TEXT, text);
  item->Flags = DIF_SEPARATOR;
  item->X1 = -1;
  item->Y1 = item->Y2 = PluginDialogBuilder::m_NextY++;
  return item;
}

FarDialogItem* PluginDialogBuilderEx::AddSeparatorEx(int text_msg_id) {
  return AddSeparatorEx(PluginDialogBuilder::GetLangString(text_msg_id));
}

FarDialogItem* PluginDialogBuilderEx::AddColorEditField(COLORREF* option_var) {
  return AddGenericEditFieldForOptionVar<PluginColorEditFieldBinding>(
             DI_FIXEDIT, kColorEditFieldValueWidth, option_var);
}

FarDialogItem* PluginDialogBuilderEx::AddStringEditField(
    std::wstring* option_var, int edit_field_width) {
  return AddGenericEditFieldForOptionVar<PluginStringEditFieldBinding>(
             DI_EDIT, edit_field_width, option_var);
}

FarDialogItem* PluginDialogBuilderEx::AddUIntEditField(
    unsigned int* option_var, int edit_field_width) {
  return AddGenericEditFieldForOptionVar<PluginUIntEditFieldBinding>(
             DI_FIXEDIT, edit_field_width, option_var);
}

FarDialogItem* PluginDialogBuilderEx::DoAddGenericEditFieldForOptionVar(
    FARDIALOGITEMTYPES field_type,
    int field_width,
    DialogAPIBindingEx* new_item_to_option_var_binding) {
  FarDialogItem* item = PluginDialogBuilder::AddDialogItem(field_type, nullptr);

  item->Data = new_item_to_option_var_binding->GetInitialValueAsStringData();
  item->Mask =
      new_item_to_option_var_binding->GenerateEditFieldMaskOnce(field_width);
  if (item->Mask)
    item->Flags |= DIF_MASKEDIT;

  PluginDialogBuilder::SetNextY(item);
  item->X2 = item->X1 + field_width - 1;

  PluginDialogBuilder::SetLastItemBinding(new_item_to_option_var_binding);
  return item;
}

}  // namespace dlgbuilderex

///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#include <array>
#include <cstring>  // for std::memset
#include <vector>

#include <Plugin.hpp>

#include "config_settings.hpp"
#include "constants.hpp"
#include "dlgbuilderex/plugin_dialog_builder_ex.hpp"
#include "globals.hpp"
#include "localized_strings_ids.hpp"
#include "version.hpp"

namespace cc_assistant {

constexpr intptr_t kCurrentEditorId = -1;

const wchar_t* GetMsg(int msg_id) {
  return g_psi().GetMsg(&g_plugin_guid, msg_id);
}

std::wstring GetEditorFileName(intptr_t editor_id) {
  const size_t filename_buffer_size =
       g_psi().EditorControl(editor_id, ECTL_GETFILENAME, 0, nullptr);
  if (filename_buffer_size == 0)
    return std::wstring();

  std::wstring filename(filename_buffer_size, 0);
  g_psi().EditorControl(editor_id, ECTL_GETFILENAME,
                        filename_buffer_size, filename.data());
  filename.resize(filename_buffer_size - 1);  // wstring adds its own null char.
  return filename;
}

bool MatchEditorFileNameWithFileMasks(intptr_t editor_id,
                                      const std::wstring& file_masks) {
  if (file_masks.empty())  // trivial case.
    return true;

  const std::wstring filename = GetEditorFileName(editor_id);

  return g_psi().FSF->ProcessName(
             file_masks.c_str(), const_cast<wchar_t*>(filename.c_str()), 0,
             PN_CMPNAMELIST | PN_SKIPPATH) != 0;
}

bool ValidateFileMasks(const std::wstring& file_masks) {
  return file_masks.empty() ||  // trivial case.
         g_psi().FSF->ProcessName(file_masks.c_str(), nullptr, 0,
                                  PN_CHECKMASK | PN_SHOWERRORMESSAGE) != 0;
}

void ActualizePluginSettingsAndRedrawEditor(intptr_t editor_id) {
  ConfigSettings::GetInstance()->ReLoadFromFarStorage();

  // Redraw the editor to visualize updated plugin settings.
  g_psi().EditorControl(editor_id, ECTL_REDRAW, 0, 0);
}

bool ShowConfigDialog() {
  dlgbuilderex::PluginDialogBuilderEx builder(
      g_psi(), g_plugin_guid, g_config_dialog_guid,
      kMConfigTitle, kConfigHelpTopic);

  ConfigSettings* config_settings = ConfigSettings::GetInstance();

  // Gather input fields references to be able to update their initial values
  // later in case of dialog re-displaying.
  std::vector<FarDialogItem*> inputs;

  // Add common 'file masks' text edit field at the top.
  constexpr int kFileMasksEditFieldWidth = 40;
  builder.AddText(kMHighlightLineLimitColumnFileMasksOption);
  inputs.push_back(
      builder.AddStringEditFieldItem(&config_settings->file_masks,
                                     kFileMasksEditFieldWidth));
  builder.AddSeparator();

  // Add "Highlight line-limit column" feature settings.
  auto& hlcs = config_settings->highlight_linelimit_column_settings;

  // Add the main setting normally.
  inputs.push_back(
      builder.AddCheckboxItem(GetMsg(kMHighlightLineLimitColumnEnabledOption),
                              &hlcs.enabled));
  builder.AddEmptyLine();

  // Add it's subsettings aligned with the main checkbox label.
  std::vector<FarDialogItem*> subitems;

  // Add line-limit column index option.
  inputs.push_back(builder.AddUIntEditFieldItem(&hlcs.column_index, 3));
  subitems.push_back(inputs.back());
  subitems.push_back(
      builder.AddTextBefore(subitems.back(),
                            kMHighlightLineLimitColumnIndexOption));

  // On the same line add the color format hint aligned with color inputs going
  // on next lines.
  FarDialogItem* color_format_hint_item =
      builder.AddTextAfter(inputs.back(), L"rrggbb");

  // Add edits for colors: fore, back and back-if-tabs - each one on its own
  // line.
  FarDialogItem* forecolor_item =
      builder.AddColorEditFieldItem(&hlcs.forecolor);
  inputs.push_back(forecolor_item);
  subitems.push_back(inputs.back());
  subitems.push_back(
      builder.AddTextBefore(subitems.back(),
                            kMHighlightLineLimitColumnForecolorOption));

  inputs.push_back(builder.AddColorEditFieldItem(&hlcs.backcolor));
  subitems.push_back(inputs.back());
  subitems.push_back(
      builder.AddTextBefore(subitems.back(),
                            kMHighlightLineLimitColumnBackcolorOption));

  inputs.push_back(builder.AddColorEditFieldItem(&hlcs.backcolor_if_tabs));
  subitems.push_back(inputs.back());
  subitems.push_back(
      builder.AddTextBefore(subitems.back(),
                            kMHighlightLineLimitColumnBackcolorIfTabsOption));

  // Finally shift all the subitems by 4 positions to right, so they are aligned
  // with the main checkbox label.
  for (FarDialogItem* item : subitems) {
    item->X1 += 4;
    item->X2 += 4;
  }

  // Then align the color format label with the color edit fields.
  const int color_format_hint_dx =
      forecolor_item->X1 - color_format_hint_item->X1;
  color_format_hint_item->X1 += color_format_hint_dx;
  color_format_hint_item->X2 += color_format_hint_dx;

  builder.AddOKCancel(kMSave, kMCancel);

  while (true) {
    if (!builder.ShowDialog()) {
      // The dialog has been cancelled: actualize settings in case they were
      // changed in another instance while user interacted with our config
      // dialog.
      ActualizePluginSettingsAndRedrawEditor(kCurrentEditorId);
      return false;
    }

    // We are going to validate 'file masks' and if incorrect then re-display
    // the config dialog with inputs filled exactly as user had them when
    // pressed 'Save' button.
    // It's important to update inputs' initial values before the validation
    // as the later may show an error message box and cause plugin settings
    // 'actualization' due focus changing between the Far windows/dialogs.
    for (FarDialogItem* item : inputs)
      builder.UpdateItemInitialValue(item);

    // Ensure 'file masks' are correct before accepting the settings.
    if (ValidateFileMasks(config_settings->file_masks))
      break;

    // Before re-displaying the config dialog do reset this instance settings
    // object to values from Far storage (which are the current actual settings
    // for the plugin).
    ActualizePluginSettingsAndRedrawEditor(kCurrentEditorId);
  }

  // Save the updated plugin settings to the Far storage.
  config_settings->SaveToFarStorage();
  return true;
}

// Show plugin menu. Returns chosen menu command index (except "Configure" which
// the function handles itself) or -1 if the menu has been cancelled.
int ShowMenuAndReturnChosenMenuIndex() {
  std::array<FarMenuItem, 1> menu_items;
  std::memset(menu_items.data(), 0, menu_items.size() * sizeof(FarMenuItem));
  menu_items[0].Text = GetMsg(kMConfigure);

  while(true) {
    const intptr_t menu_result_code =
        g_psi().Menu(&g_plugin_guid, nullptr, -1, -1, 0, FMENU_WRAPMODE,
                     GetMsg(kMTitle), nullptr, kMenuHelpTopic, nullptr, nullptr,
                     menu_items.data(), menu_items.size());

    const int chosen_menu_index = static_cast<int>(menu_result_code);
    if (chosen_menu_index != 0)  // filter "Configure" item index.
      return chosen_menu_index;

    ShowConfigDialog();
  }
  return -1;
}

// Parse a string command passed by a Lua macro then convert to corresponding
// menu command id.
int GetCommandIdForMacroString(const wchar_t* macro_string_value) {
  int result_command_id = -1;

  auto check_known_command =
      [macro_string_value, &result_command_id](
          const wchar_t* known_command_name, int known_command_id) {
        if (lstrcmpi(macro_string_value, known_command_name) != 0)
          return false;

        result_command_id = known_command_id;
        return true;
      };

  check_known_command(L"Configure", 0);
  return result_command_id;
}

void HighlightLineLimitColumnIfEnabled(intptr_t editor_id) {
  ConfigSettings* config_settings = ConfigSettings::GetInstance();

  auto& hlcs = config_settings->highlight_linelimit_column_settings;

  if (!hlcs.enabled)
    return;

  // TODO: add optimization here.
  if (!MatchEditorFileNameWithFileMasks(editor_id,
                                        config_settings->file_masks)) {
    return;
  }

  EditorInfo editor_info = { sizeof(EditorInfo) };
  g_psi().EditorControl(editor_id, ECTL_GETINFO, 0, &editor_info);

  // Optimization: do nothing if the column is out of screen at all.
  if (editor_info.LeftPos > hlcs.column_index ||
      editor_info.LeftPos + editor_info.WindowSizeX < hlcs.column_index) {
    return;
  }

  for (intptr_t i = 0; i < editor_info.WindowSizeY; ++i) {
    const intptr_t curr_visible_line_index = editor_info.TopScreenLine + i;
    if (curr_visible_line_index >= editor_info.TotalLines)
      break;

    // Watch for tabs in the line: ensure the target column is right.
    EditorConvertPos ecp = { sizeof(EditorConvertPos) };
    ecp.StringNumber = curr_visible_line_index;
    ecp.SrcPos = hlcs.column_index;
    g_psi().EditorControl(editor_id, ECTL_TABTOREAL, 0, &ecp);
    const intptr_t adjusted_column_index = ecp.DestPos;
    const bool tabs_detected = (adjusted_column_index != hlcs.column_index);

    EditorColor ec = { sizeof(EditorColor) };
    ec.StringNumber = curr_visible_line_index;
    ec.ColorItem = -1;
    ec.StartPos = adjusted_column_index;
    ec.EndPos = adjusted_column_index;
    ec.Priority = EDITOR_COLOR_NORMAL_PRIORITY;
    ec.Flags = ECF_AUTODELETE;
    ec.Color.Flags = 0;
    ec.Color.ForegroundColor = hlcs.forecolor;
    ec.Color.BackgroundColor = (tabs_detected) ? hlcs.backcolor_if_tabs
                                               : hlcs.backcolor; 
    ec.Owner = g_plugin_guid;
    g_psi().EditorControl(editor_id, ECTL_ADDCOLOR, 0, &ec);
  }
}

}  // namespace cc_assistant

////////////////////////////////////////////////////////////////////////////////
// Plugin DLL exported routines for calls made by Far.
// 

extern "C" void WINAPI GetGlobalInfoW(GlobalInfo* info) {
  info->StructSize = sizeof(GlobalInfo);
  info->MinFarVersion = FARMANAGERVERSION;
  info->Version = PLUGIN_VERSION;
  info->Guid = cc_assistant::g_plugin_guid;
  info->Title = PLUGIN_NAME;
  info->Description = PLUGIN_DESC;
  info->Author = PLUGIN_AUTHOR;
}

extern "C" void WINAPI GetPluginInfoW(PluginInfo* info) {
  info->StructSize = sizeof(PluginInfo);
  info->Flags = PF_EDITOR | PF_DISABLEPANELS;

  static const wchar_t* static_title_string =
      cc_assistant::GetMsg(cc_assistant::kMTitle);
  info->PluginMenu.Guids = &cc_assistant::g_menu_guid;
  info->PluginMenu.Strings = &static_title_string;
  info->PluginMenu.Count = 1;

  info->PluginConfig.Guids = &cc_assistant::g_config_dialog_guid;
  info->PluginConfig.Strings = &static_title_string;
  info->PluginConfig.Count = 1;
}

extern "C" void WINAPI SetStartupInfoW(const PluginStartupInfo* info) {
  // Initialize this plugin instance's copy of the |info| object provided by
  // Far. (See g_psi() function notes in globals.hpp for more details.)
  cc_assistant::g_psi(info);
}

extern "C" intptr_t WINAPI ConfigureW(const ConfigureInfo* info) {
  // As it's an external call, firstly ensure we have the latest plugin
  // settings from the Far storage - they could be changed by another Far
  // instance.
  cc_assistant::ActualizePluginSettingsAndRedrawEditor(
      cc_assistant::kCurrentEditorId);

  return cc_assistant::ShowConfigDialog() ? 1 : 0;
}

extern "C" HANDLE WINAPI OpenW(const OpenInfo* open_info) {
  // As it's an external call, firstly ensure we have the latest plugin
  // settings from the Far storage - they could be changed by another Far
  // instance.
  cc_assistant::ActualizePluginSettingsAndRedrawEditor(
      cc_assistant::kCurrentEditorId);

  // Plugin is called to do an action: chose behaviour depending on where it
  // comes from.
  int command_id = -1;
  switch (open_info->OpenFrom) {
    // An editor wants to show the plugin menu.
    case OPEN_EDITOR:
      command_id = cc_assistant::ShowMenuAndReturnChosenMenuIndex();
      break;

    // Plugin cmd prefix was detected: do handle the command line.
    case OPEN_COMMANDLINE:
      break;

    // A Lua macro has called the plugin (e.g. a key shortcut was triggered). 
    case OPEN_FROMMACRO: {
        OpenMacroInfo* macro_info = (OpenMacroInfo*)open_info->Data;
        if (macro_info->Count > 0) {
          switch (macro_info->Values[0].Type) {
            case FMVT_INTEGER:
              command_id = static_cast<int>(macro_info->Values[0].Integer);
              break;

            case FMVT_DOUBLE:
              command_id = static_cast<int>(macro_info->Values[0].Double);
              break;

            case FMVT_STRING:
              command_id = cc_assistant::GetCommandIdForMacroString(
                  macro_info->Values[0].String);
              break;

            default:
              break;
          }
        }

        // "Configure" is the only macro command we support in the plugin.
        // Here handle it directly as ShowMenuAndReturnChosenMenuIndex() filters
        // it.
        if (command_id == 0) {
          cc_assistant::ShowConfigDialog();
        } else if (command_id < 0) {
          // If macro command id has not been determined then show plugin menu
          // to select a known command by UI.
          command_id = cc_assistant::ShowMenuAndReturnChosenMenuIndex();
        }
      }
      break;

    default:
      break;
  }

  // Handle all possible command ids except "Configure" one.
  // switch(command_id) {
  // }

  return (open_info->OpenFrom == OPEN_FROMMACRO) ? INVALID_HANDLE_VALUE
                                                 : nullptr;
}

extern "C" intptr_t WINAPI ProcessEditorEventW(
    const ProcessEditorEventInfo* info) {
  switch (info->Event) {
    case EE_REDRAW:
      cc_assistant::HighlightLineLimitColumnIfEnabled(info->EditorID);
      break;

    case EE_GOTFOCUS:
      // Actualize plugin settings from the Far storage as another plugin
      // instance (in another Far process) could change them while this
      // instance was in "background".
      cc_assistant::ActualizePluginSettingsAndRedrawEditor(info->EditorID);
      break;

    default:
      break;
  }

  return 0;
}

// Avoid loading into Far2.
extern "C" int WINAPI GetMinFarVersionW() {
  #define MAKEFARVERSION_IN_OLD_FORMAT(major, minor, build) \
      (((build) << 16) | (((major) & 0xff) << 8) | ((minor) & 0xff))

  return MAKEFARVERSION_IN_OLD_FORMAT(FARMANAGERVERSION_MAJOR,
                                      FARMANAGERVERSION_MINOR,
                                      FARMANAGERVERSION_BUILD);
}

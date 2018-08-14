////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#include <algorithm>
#include <array>
#include <cstring>

#include <DlgBuilder.hpp>
#include <Plugin.hpp>
#include <PluginSettings.hpp>

#include <initguid.h>
#include "guid.hpp"

#include "config_settings.hpp"
#include "localized_strings_ids.hpp"
#include "version.hpp"

namespace {

PluginStartupInfo g_info;
FarStandardFunctions g_fsf;
cc_assistant::ConfigSettings g_opt;

const wchar_t* GetMsg(int msg_id) {
  return g_info.GetMsg(&MainGuid, msg_id);
}

}  // namespace

namespace cc_assistant {

bool ShowConfigDialog() {
  PluginDialogBuilder Builder(
      g_info, MainGuid, ConfigGuid, kMTitleConfig, nullptr);

  // Add "highlight line-limit column" controls.
  Builder.AddCheckbox(kMHighlightLineLimitColumnOption,
                      &g_opt.highlight_linelimit_column);
  FarDialogItem* linelimit_editbox =
      Builder.AddUIntEditField(&g_opt.highlight_linelimit_column_index, 3);
  FarDialogItem* linelimit_label =
      Builder.AddTextBefore(linelimit_editbox,
                            kMHighlightLineLimitColumnIndexOption);
  // Place the line-limit controls right under main option label.
  linelimit_label->X1 += 4;
  linelimit_label->X2 += 4;
  // Passing 3 here to remove 'weird' space between the controls.
  linelimit_editbox->X1 += 3;
  linelimit_editbox->X2 += 3;
  Builder.AddSeparator();

  // Add "detect whitespaces at line ends" controls.
  Builder.AddCheckbox(kMDetectWhitespacesAtLineEndsOption,
                      &g_opt.detect_whitespaces_at_line_ends);

  // Add standard dialog buttons.
  Builder.AddOKCancel(kMSave, kMCancel);

  if (!Builder.ShowDialog())
    return false;  // the dialog has been cancelled.

  // Save the updated plugin settings to the Far storage.
  PluginSettings far_settings_storage(MainGuid, g_info.SettingsControl);
  g_opt.SaveToFarStorage(&far_settings_storage);
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
        g_info.Menu(&MainGuid, nullptr, -1, -1, 0, FMENU_WRAPMODE,
                    GetMsg(kMTitle), nullptr, L"Contents", nullptr, nullptr,
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

void HighlightLineLimitColumn(intptr_t editor_id) {
  EditorInfo editor_info = { sizeof(EditorInfo) };
  g_info.EditorControl(editor_id, ECTL_GETINFO, 0, &editor_info);

  for (intptr_t i = 0; i < editor_info.WindowSizeY; ++i) {
    const intptr_t curr_visible_line_index = editor_info.TopScreenLine + i;
    if (curr_visible_line_index >= editor_info.TotalLines)
      break;

    EditorColor ec = { sizeof(EditorColor) };
    ec.StringNumber = curr_visible_line_index;
    ec.ColorItem = -1;
    ec.StartPos = g_opt.highlight_linelimit_column_index;
    ec.EndPos = g_opt.highlight_linelimit_column_index;
    ec.Priority = EDITOR_COLOR_NORMAL_PRIORITY;
    ec.Flags = ECF_TABMARKFIRST | ECF_AUTODELETE;
    ec.Color.Flags = FCF_FG_4BIT | FCF_BG_4BIT;
    ec.Color.ForegroundColor = 0x701050F0;  // ABGR -> red.
    ec.Color.BackgroundColor = 0x00000000;  // ABGR -> black. 
    ec.Owner = MainGuid;
    g_info.EditorControl(editor_id, ECTL_ADDCOLOR, 0, &ec);
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
  info->Guid = MainGuid;
  info->Title = PLUGIN_NAME;
  info->Description = PLUGIN_DESC;
  info->Author = PLUGIN_AUTHOR;
}

extern "C" void WINAPI GetPluginInfoW(PluginInfo* info) {
  info->StructSize = sizeof(PluginInfo);
  info->Flags = PF_EDITOR | PF_DISABLEPANELS;

  static const wchar_t* eternal_title_string = GetMsg(cc_assistant::kMTitle);
  info->PluginMenu.Guids = &MenuGuid;
  info->PluginMenu.Strings = &eternal_title_string;
  info->PluginMenu.Count = 1;

  info->PluginConfig.Guids = &ConfigGuid;
  info->PluginConfig.Strings = &eternal_title_string;
  info->PluginConfig.Count = 1;
}

extern "C" void WINAPI SetStartupInfoW(const PluginStartupInfo* info) {
  // Use a local copy of |info| as the object will be deleted after the call.
  g_info = (*info);
  g_fsf = (*info->FSF);
  g_info.FSF = &g_fsf;

  // Load current plugin settings from the Far storage.
  PluginSettings far_settings_storage(MainGuid, g_info.SettingsControl);
  g_opt.LoadFromFarStorage(far_settings_storage);
}

extern "C" intptr_t WINAPI ConfigureW(const ConfigureInfo* info) {
  return cc_assistant::ShowConfigDialog() ? 1 : 0;
}

extern "C" HANDLE WINAPI OpenW(const OpenInfo* open_info) {
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
      cc_assistant::HighlightLineLimitColumn(info->EditorID);
      return 1;

    default:
      break;
  }

  return 0;
}

extern "C" intptr_t WINAPI ProcessEditorInputW(
    const ProcessEditorInputInfo* info) {
  return 0;
}

// Free all the internal structures on shutdown.
extern "C" void WINAPI ExitFARW(const ExitInfo* info) {
}

// Avoid loading into Far2.
extern "C" int WINAPI GetMinFarVersionW() {
  #define MAKEFARVERSION_IN_OLD_FORMAT(major, minor, build) \
      (((build) << 16) | (((major) & 0xff) << 8) | ((minor) & 0xff))

  return MAKEFARVERSION_IN_OLD_FORMAT(FARMANAGERVERSION_MAJOR,
                                      FARMANAGERVERSION_MINOR,
                                      FARMANAGERVERSION_BUILD);
}

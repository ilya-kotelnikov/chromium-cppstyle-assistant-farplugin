////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#pragma once

#include <DlgBuilder.hpp>
#include <Plugin.hpp>

namespace dlgbuilderex {

// Define additional interface methods for all of our bindings to enable generic
// algorithms.
//
class ItemBinding : public DialogAPIBinding {
 public:
  ItemBinding(const PluginStartupInfo& plugin_startup_info,
              HANDLE* dialog_handle, int item_id)
      : DialogAPIBinding(plugin_startup_info, dialog_handle, item_id) {
  }

  virtual void UpdateItemInitialValue(FarDialogItem* item) = 0;
};

}  // namespace dlgbuilderex

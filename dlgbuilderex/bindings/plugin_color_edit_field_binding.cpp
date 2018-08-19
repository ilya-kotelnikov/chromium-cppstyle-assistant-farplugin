////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#include "dlgbuilderex/bindings/plugin_color_edit_field_binding.hpp"

#include <stdint.h>
#include <strsafe.h>

const wchar_t kColorEditFieldValueMask[] = L"HHHHHH";  // 'RRGGBB' hex.

namespace {

void SetColorValueToHexString(COLORREF value, wchar_t* buffer, size_t size);

bool GetColorValueFromHexString(COLORREF* value,
                                const wchar_t* hex_rrggbb_string);

}  // namespace

PluginColorEditFieldBinding::PluginColorEditFieldBinding(
    const PluginStartupInfo& plugin_startup_info,
    HANDLE* dialog_handle, int item_id, COLORREF* option_var)
    : PluginGenericEditFieldBinding<COLORREF>(
          plugin_startup_info, dialog_handle, item_id, option_var) {
  SetColorValueToHexString(*option_var_,
                           initial_value_as_string_,
                           kColorEditFieldValueWidth + 1);
}

const wchar_t*
PluginColorEditFieldBinding::GetInitialValueAsStringData() const {
  return initial_value_as_string_;
}

void PluginColorEditFieldBinding::SetResultValueFromStringData(
    const wchar_t* data) const {
  GetColorValueFromHexString(option_var_, data);
}

////////////////////////////////////////////////////////////////////////////////
// Color conversion utilities implemenation
//
namespace {

struct ColorExploder {
  union {
    COLORREF color;
    struct {
      unsigned char r, g, b, a;
    } rgba;
  };
};

void SetColorValueToHexString(COLORREF value, wchar_t* buffer, size_t size) {
  ColorExploder ce;
  ce.color = value;
  // TODO: use g_info.FSF->sprintf here.
  ::StringCchPrintfW(buffer, size,
                     L"%02X%02X%02X", ce.rgba.r, ce.rgba.g, ce.rgba.b);
}

bool GetColorValueFromHexString(COLORREF* value,
                                const wchar_t* hex_rrggbb_string) {
  auto hex_char_to_halfbyte =
      [](wchar_t hc, uint8_t* hb) -> uint8_t {
        if (hc >= '0' && hc <= '9')
          (*hb) = 0x0 + hc - '0';
        else if (hc >= 'a' && hc <= 'f')
          (*hb) = 0xA + hc - 'a';
        else if (hc >= 'A' && hc <= 'F')
          (*hb) = 0xA + hc - 'A';
        else
          return false;  // hc == 0 or not a hex char.

        return true;
      };

  bool may_read_more = true;
  auto read_two_hex_chars_to_byte =
      [&may_read_more, &hex_char_to_halfbyte](const wchar_t** p) -> uint8_t {
        uint8_t hb0, hb1; 
        may_read_more = may_read_more && hex_char_to_halfbyte(**p, &hb0) &&
                                         hex_char_to_halfbyte(*++(*p), &hb1);
        if (!may_read_more)
          return 0;
        // This pair of chars is valid so it's safe to shift the input pointer.
        ++(*p);
        return (hb0 << 4) | hb1;
      };

  const wchar_t* reading_input_ptr = hex_rrggbb_string;

  ColorExploder ce;
  ce.rgba.r = read_two_hex_chars_to_byte(&reading_input_ptr);
  ce.rgba.g = read_two_hex_chars_to_byte(&reading_input_ptr);
  ce.rgba.b = read_two_hex_chars_to_byte(&reading_input_ptr);

  // The input string must be read fully (i.e. reading ends at the null char)
  // and be of the expected length.
  if ((*reading_input_ptr) != 0 ||
      reading_input_ptr - hex_rrggbb_string != kColorEditFieldValueWidth) {
    return false;
  }

  (*value) = ce.color;
  return true;
}

}  // namespace

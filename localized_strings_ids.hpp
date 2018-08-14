////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#pragma once

namespace cc_assistant {

// Keep the indexes consistent with translations in "distrib/Options*.lng"
// files.
enum {
  kMTitle,

  kMHighlightLineLimitColumn,
  kMDetectWhitespacesAtLineEnds,

  kMConfigure,

  kMTitleConfig,
  kMHighlightLineLimitColumnOption,
  kMHighlightLineLimitColumnIndexOption,
  kMDetectWhitespacesAtLineEndsOption,

  kMSave,
  kMCancel,
};

}  // namespace cc_assistant

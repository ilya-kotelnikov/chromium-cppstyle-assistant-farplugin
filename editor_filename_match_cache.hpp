////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#pragma once

#include <map>
#include <string>

namespace cc_assistant {

class EditorFileNameMatchCache {
 public:
  EditorFileNameMatchCache();
  ~EditorFileNameMatchCache();

  void SetFileMasks(const std::wstring& file_masks);

  void SetEditorFileName(intptr_t editor_id,
                         const wchar_t* saved_as_filepath = nullptr);
  void DropEditor(intptr_t editor_id);

  bool GetEditorMatchOrFileNameRef(intptr_t editor_id,
                                   bool* match,
                                   const std::wstring** filename_ref);
  void SetEditorMatch(intptr_t editor_id, bool match);

 private:
  enum class MatchState {
    kUnknown,
    kMatch,
    kClash,  // ~ not match :)
  };

  struct MatchData {
    std::wstring filename;
    MatchState match_state;

    MatchData(std::wstring&& filename, MatchState match_state)
        : filename(filename), match_state(match_state) {
    }
  };

  MatchData* FindEditorMatchDataOrAdd(intptr_t editor_id);
  std::wstring AskEditorForFileName(intptr_t editor_id);
  std::wstring ExtractFileName(std::wstring&& filepath);

  std::wstring file_masks_;
  std::map<intptr_t, MatchData> editor_to_match_map_;
};

}  // namespace cc_assistant

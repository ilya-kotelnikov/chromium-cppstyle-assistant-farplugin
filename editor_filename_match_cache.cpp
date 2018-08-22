////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018, Ilya Kotelnikov (ilya.kotelnikov@gmail.com)
// All rights reserved.
//
// You may use, distribute and modify this code under the terms of GNU GPLv3.
//------------------------------------------------------------------------------

#include "editor_filename_match_cache.hpp"

#include <utility>

#include <Plugin.hpp>

#include "globals.hpp"

namespace cc_assistant {

EditorFileNameMatchCache::EditorFileNameMatchCache() {
}

EditorFileNameMatchCache::~EditorFileNameMatchCache() {
}

void EditorFileNameMatchCache::SetFileMasks(const std::wstring& file_masks) {
  if (file_masks_ == file_masks)
    return;

  file_masks_ = file_masks;

  // The file masks have been changed -> drop all the matches.
  editor_to_match_map_.clear();
}

void EditorFileNameMatchCache::SetEditorFileName(
    intptr_t editor_id, const wchar_t* saved_as_filepath) {
  std::wstring filename =
      (saved_as_filepath) ? ExtractFileName(saved_as_filepath)
                          : AskEditorForFileName(editor_id);

  auto it = editor_to_match_map_.find(editor_id);
  if (it == editor_to_match_map_.end()) {
    editor_to_match_map_.insert(std::make_pair(
        editor_id,
        MatchData(std::move(filename), MatchState::kUnknown)));
    return;
  }

  // Reset editor's match if its filename has changed.
  if (it->second.filename != filename) {
    it->second.filename = std::move(filename);
    it->second.match_state = MatchState::kUnknown;
  }
}

void EditorFileNameMatchCache::DropEditor(intptr_t editor_id) {
  editor_to_match_map_.erase(editor_id);
}

bool EditorFileNameMatchCache::GetEditorMatchOrFileNameRef(
    intptr_t editor_id, bool* match, const std::wstring** filename_ref) {
  MatchData* match_data = FindEditorMatchDataOrAdd(editor_id);

  if (match_data->match_state == MatchState::kUnknown) {
    (*filename_ref) = &match_data->filename;
    return false;
  }

  *match = (match_data->match_state == MatchState::kMatch);
  return true;
}

void EditorFileNameMatchCache::SetEditorMatch(intptr_t editor_id, bool match) {
  MatchData* match_data = FindEditorMatchDataOrAdd(editor_id);

  match_data->match_state = (match) ? MatchState::kMatch : MatchState::kClash;
}

EditorFileNameMatchCache::MatchData*
EditorFileNameMatchCache::FindEditorMatchDataOrAdd(intptr_t editor_id) {
  auto it = editor_to_match_map_.find(editor_id);
  if (it != editor_to_match_map_.end())
    return &it->second;

  // It may happen that there is no data for an editor: the cache object may be
  // re-created when user changes settings. So if the editor is unknown, then
  // just add it with current filename.
  auto result = editor_to_match_map_.insert(
      std::make_pair(editor_id, MatchData(AskEditorForFileName(editor_id),
                                          MatchState::kUnknown)));
  return &result.first->second;
}

std::wstring EditorFileNameMatchCache::AskEditorForFileName(
    intptr_t editor_id) {
  const size_t filepath_buffer_size =
       g_psi().EditorControl(editor_id, ECTL_GETFILENAME, 0, nullptr);
  if (filepath_buffer_size == 0)
    return std::wstring();

  std::wstring filepath(filepath_buffer_size, 0);
  g_psi().EditorControl(editor_id, ECTL_GETFILENAME,
                        filepath_buffer_size, filepath.data());
  filepath.resize(filepath_buffer_size - 1);  // wstring adds its own null char.

  // Cut the path - we use only the filename (a portion after the last slash).
  return ExtractFileName(std::move(filepath));
}

std::wstring EditorFileNameMatchCache::ExtractFileName(
    std::wstring&& filepath) {
  const size_t last_slash_pos = filepath.find_last_of(L"/\\");
  return (last_slash_pos == std::wstring::npos)
             ? std::move(filepath) : filepath.substr(last_slash_pos + 1);
}

}  // namespace cc_assistant

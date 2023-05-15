//
// Created by Tobiathan on 4/23/23.
//

#include "FilePathInput.h"
#include <UICommon.h>
#include "TextInput.h"
#include "ZeroPoint.h"
#include <filesystem>

namespace fs = std::filesystem;

vector<string> SuggestionsFor(const fs::path& from, const string& text) {
    const auto IsValidSuggestion = [&](const string& pathStr) {
        return pathStr.starts_with(text);
    };

    return fs::directory_iterator(from) | MAP_CALL(.path()) | MAP_FUNC(string)
        | filter(IsValidSuggestion) | tutil::to_vector{};
}

vector<string> FilePathSuggestions(const string& text) {
    if (fs::is_directory(text)) {
        return SuggestionsFor(text, text);
    }

    if (text.empty() && fs::is_directory("/")) {
        return SuggestionsFor("/", text);
    }

    auto lastSlash = text.find_last_of('/'); // TODO: toby, support both types of slashes??? better lib for this??
    if (lastSlash != string::npos) {
        string partial = text.substr(0, lastSlash + 1);
        if (fs::is_directory(partial)) {
            return SuggestionsFor(partial, text);
        }
    }

    return {};
}

struct FilePathInputState {
    optional<string> prevText = std::nullopt;
    vector<string> suggestions = {};
    bool inputWasFocused = false;
    int selectedSuggestionIndex = 0;
};

void tui::FilePathInput(string& text, const FilePathInputConfig& config) {
    auto& my = UseRef<FilePathInputState>({});
    optional<string> suggestionToUse = std::nullopt;

    if (!my.prevText || text != *my.prevText) { // onDiff(text)
        my.suggestions = FilePathSuggestions(text);
        my.selectedSuggestionIndex = 0;
        my.prevText = text;
    }

    Div container (config.containerStyle);
    TextInput input (text, config.inputStyle.withStyle({
        .borderColor = (fs::is_directory(text)) ? GREEN : RED
    }));

    ZeroPoint zp;

    if (my.inputWasFocused && !my.suggestions.empty()) {
        ScrollViewY suggestionContainer (config.suggestionContainer);
        for (const string& suggestion : my.suggestions) {
            ScopeId suggestionId (suggestion);

            bool selected = suggestion == my.suggestions[my.selectedSuggestionIndex];
            Interactive suggestionBox (
                    OptStyles{ config.suggestionBox, {{ selected, config.suggestionBoxActive }} },
                    { selected ? style_t{} : config.suggestionBoxHover, config.suggestionBoxActive });
            if (suggestionBox.Pressed()) {
                suggestionToUse = suggestion;
            }

            Span suggestionSpan;
            Text(text, config.suggestTextMatch);
            Text(suggestion.substr(std::min(suggestion.size(), text.size())), config.suggestTextNonMatch);
        }
    }

    if (KeyBinds::Pressed(KeyBind::ARROW_UP)) {
        my.selectedSuggestionIndex = std::max(my.selectedSuggestionIndex - 1, 0);
    }
    if (KeyBinds::Pressed(KeyBind::ARROW_DOWN)) {
        my.selectedSuggestionIndex = std::min(my.selectedSuggestionIndex + 1, (int) my.suggestions.size() - 1);
    }

    if (KeyBinds::Pressed(KeyBind::INPUT_ENTER)) {
        suggestionToUse = my.suggestions[my.selectedSuggestionIndex];
    }

    if (suggestionToUse) {
        input.SetFullText(fs::is_directory(*suggestionToUse) ? (*suggestionToUse + "/") : *suggestionToUse);
        input.Focus();
    }

    my.inputWasFocused = input.IsFocused();
}

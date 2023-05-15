//
// Created by Tobiathan on 4/23/23.
//

#pragma once

#include <Includes.h>
#include "Style.h"

namespace tui {
    struct FilePathInputConfig {
        style_t containerStyle;
        style_t inputStyle;
        style_t suggestionContainer;
        style_t suggestionBox;
        style_t suggestionBoxHover;
        style_t suggestionBoxActive;
        style_t suggestTextMatch;
        style_t suggestTextNonMatch;
    };

    void FilePathInput(string& text, const FilePathInputConfig& config = {});
}

//
// Created by Tobiathan on 3/6/23.
//

#pragma once

#include "Includes.h"
#include "UIComponent.h"
#include "FilePickerDirectoryNode.h"

namespace tui {
    struct FilePickerDirectory : public UIComponent {
        explicit FilePickerDirectory(FilePickerDirectoryNode& node, int depth = 0, const id_t& idLike = "", tloc location = tloc::current());

    private:
        static constexpr int INDENT_PIXELS = 18;

        static CStyle IndentClass(const string& classes, int indents);
    };
}

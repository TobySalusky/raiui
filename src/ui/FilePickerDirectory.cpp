//
// Created by Tobiathan on 3/6/23.
//

#include "FilePickerDirectory.h"
#include "State.h"
#include "Div.h"
#include "Interactive.h"
#include "Button.h"
#include "Span.h"
#include "Leaf.h"

tui::FilePickerDirectory::FilePickerDirectory(FilePickerDirectoryNode& node, int depth, const tui::id_t& idLike, tloc location) : UIComponent(idLike, location) {
    ScopeId scopeId(id);

    {
        Div container ("directory-container");
        Interactive dirLine (
                IndentClass("directory-line", depth),
                InteractiveStyles { "directory-line-highlight", "directory-line-active" }
        );
        if (!node.children.empty()) {
            Button caretButton;
            Text(node.open ? "v" : ">", CStyle{"file-picker-text", {.paddingHoriz = 5}});
            if (caretButton.Pressed() || dirLine.DoublePressed()) {
                node.Toggle();
            }
        } else {
            Text(" ", CStyle{"file-picker-text", {.paddingHoriz = 5}});
        }
        Leaf("file-picker-folder-icon");
        Text(node.name, "file-picker-text");
    }

    if (!node.open) { return; }

    for (auto& child : node.children) {
        FilePickerDirectory subDir(child, depth + 1, id_append { child.path });
    }
}

CStyle tui::FilePickerDirectory::IndentClass(const string& classes, int indents) {
    return { classes, { .paddingLeft = indents * INDENT_PIXELS } };
}
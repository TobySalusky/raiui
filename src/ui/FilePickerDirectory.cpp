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

        bool hasChildren = !node.children.empty();
        { // caret and toggling functionality
            Button caretButton;

            string buttonText =
                hasChildren
                    ? (node.open ? "v" : ">")
                    : " ";

            Text(buttonText, CStyle{"file-picker-text", {.paddingHoriz = 5}});

            if (hasChildren && (caretButton.Pressed() || dirLine.DoublePressed())) {
                node.Toggle();
            }
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
//
// Created by Tobiathan on 3/6/23.
//

#include "FilePickerDirectory.h"
#include "State.h"
#include "Div.h"
#include "Interactive.h"
#include "Button.h"
#include "Span.h"

tui::FilePickerDirectory::FilePickerDirectory(FilePickerDirectoryNode& node, int depth, const tui::id_t& idLike, tloc location) : UIComponent(idLike, location) {
    ScopeId scopeId(id);

    {
        // TODO: NOTE: no early returns in function components!!!! gets very messy!!
        Div container ("directory-container"); // placing this in the outer scope is UB?? TODO: figure out why!! (because return on open???? -- variable stack size???)
        Interactive dirLine (
                IndentClass("directory-line", depth),
                InteractiveStyles { "directory-line-highlight", "directory-line-active" }
        );
        if (!node.children.empty()) {
            Button caretButton;
            Text(node.open ? "v" : "^", CStyle{"file-picker-text", {.paddingHoriz = 5}});
            if (caretButton.Pressed() || dirLine.DoublePressed()) {
                node.Toggle();
            }
        } else {
            Text(" ", CStyle{"file-picker-text", {.paddingHoriz = 5}});
        }
//        Div icon("file-picker-folder-icon");

        Text(node.name, "file-picker-text");
    }

    if (node.open) { // DO NOT MAKE EARLY RETURN!
        for (auto& child : node.children) {
            FilePickerDirectory subDir(child, depth + 1, id_append { child.path });
        }
    }
}

CStyle tui::FilePickerDirectory::IndentClass(const string& classes, int indents) {
    return { classes, { .paddingLeft = indents * INDENT_PIXELS } };
}
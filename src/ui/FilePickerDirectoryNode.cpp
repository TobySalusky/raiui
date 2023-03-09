//
// Created by Tobiathan on 3/6/23.
//

#include "FilePickerDirectoryNode.h"
tui::FilePickerDirectoryNode tui::FilePickerDirectoryNode::Create(const fs::path& p) {
    FilePickerDirectoryNode node = { p, p.filename() };

    auto dirIter = fs::directory_iterator(p);
    for (auto& entry : dirIter) {
        if (entry.is_directory()) {
            node.children.push_back(Create(entry.path()));
        }
    }

    return node;
}

void tui::FilePickerDirectoryNode::Toggle() {
    if (open) { Close(); } else { Open(); }
}

void tui::FilePickerDirectoryNode::Open() {
    open = true;
}

void tui::FilePickerDirectoryNode::Close() {
    open = false;
    for (auto& child : children) {
        child.Close();
    }
}

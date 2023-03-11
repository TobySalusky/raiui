//
// Created by Tobiathan on 3/6/23.
//

#pragma once

#include "Includes.h"
#include <filesystem>
namespace fs = std::filesystem;

namespace tui {
    struct FilePickerDirectoryNode {
        string path;
        string name;
        vector<FilePickerDirectoryNode> children = {};
        bool open = false;

        void Toggle();

        // factory func
        static FilePickerDirectoryNode Create(const fs::path& p);

    private:
        void Open();
        void Close();
    };
}